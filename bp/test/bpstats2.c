/*
 * bpstats2.c
 * Very heavily based on bpstats, in the original ION distribution.
 * Andrew Jenkins <andrew.jenkins@colorado.edu>
 * A BP client that fills a bundle with statistics about the current BPA
 * and sends it.
 */

#include <stdlib.h>
#include "bpP.h"
#include <bp.h>

static BpSAP        sap;
static BpVdb        *vdb;
static Sdr          sdr;
char *defaultDestEid = NULL;
char *ownEid = NULL;
static BpCustodySwitch custodySwitch = NoCustodyRequested;
char   theBuffer[2048];
static int needSendDefault = 0;
static int needShutdown = 0;

const char usage[] = 
"Usage: bpstats2 <source EID> [<default dest EID>] [ct]\n\n"
"Replies to any bundles it receives with a bundle containing the statistics\n"
"of the BPA to which it is attached.\n\n"
"If a default destination EID is specified, then statistics bundles can be\n"
"triggered to be sent to that EID by sending SIGUSR1 to bpstats2.\n"
"If ct specified, the bundles are sent with custody transfer.\n";

void handleQuit(int sig)
{
	needShutdown = 1;
	bp_interrupt(sap);
}

void sendDefault(int sig)
{
	needSendDefault = 1;
	bp_interrupt(sap);
}

/* This is basically libbpP.c's "reportStateStats()" except to a buffer. */
int appendStateStats(char *buffer, size_t len, int stateIdx)
{
	static char *classnames[] = 
	{ "src", "fwd", "xmt", "rcv", "dlv", "ctr", "ctt", "exp" };
	time_t startTime;
	time_t currentTime;
	BpStateStats *state;

	if(stateIdx < 0 || stateIdx > 7) { return -1; }

	currentTime = getUTCTime();
	startTime = vdb->statsStartTime;
	state = &(vdb->stateStats[stateIdx]);

	return snprintf(buffer, len, "  [x] %s from %u to %u: (0) %u %lu (1) %u %lu \
(2) %u %lu (@) %u %lu\n", classnames[stateIdx], (unsigned int) startTime,
			(unsigned int) currentTime,
			state->stats[0].bundles, state->stats[0].bytes,
			state->stats[1].bundles, state->stats[1].bytes,
			state->stats[2].bundles, state->stats[2].bytes,
			state->stats[3].bundles, state->stats[3].bytes);
}

int sendStats(char *destEid, char *buffer, size_t len)
{
	int bytesWritten = 0, rc;
	int i = 0;
	Object bundleZco, extent;
	Object newBundle;   /* We never use but bp_send requires it. */

	if(destEid == NULL || (strcmp(destEid, "dtn:none") == 0)) {
		putErrmsg("Can't send stats: bad dest EID", destEid);
		return -1;
	}

	/* Write the stats to a buffer. */
	rc = snprintf(buffer, len, "stats %s: \n",  ownEid);
	if(rc < 0) return -1;
	bytesWritten += rc;

	for(i = 0; bytesWritten < len && i < 8; i++) {
		rc = appendStateStats(buffer + bytesWritten, len - bytesWritten, i);
		if(rc < 0) return -1;
		bytesWritten += rc;
	}

	/* Wrap bundleZco around the stats buffer. */
	sdr_begin_xn(sdr);
	extent = sdr_malloc(sdr, bytesWritten);
	if(extent == 0) {
		sdr_cancel_xn(sdr);
		putSysErrmsg("No space for ZCO extent", NULL);
		return -1;
	}

	sdr_write(sdr, extent, buffer, bytesWritten);
	bundleZco = zco_create(sdr, ZcoSdrSource, extent, 0, bytesWritten);
	if(sdr_end_xn(sdr) < 0 || bundleZco == 0)
	{
		putErrmsg("Can't create ZCO.", NULL);
		return -1;
	}

	/* Send bundleZco, the stats bundle. */
	if(bp_send(sap, BP_BLOCKING, destEid, NULL, 86400, BP_STD_PRIORITY,
				custodySwitch, 0, 0, NULL, bundleZco, &newBundle) < 0)
	{
		putSysErrmsg("bpstats2 can't send stats bundle.", NULL);
		return -1;
	}
	return 0;
}

int main(int argc, char **argv)
{
	ownEid          = (argc > 1 ? argv[1] : NULL);
	defaultDestEid  = (argc > 2 ? argv[2] : NULL);
	char *ctArg     = (argc > 3 ? argv[3] : NULL);
	BpDelivery      dlv;


	if(argc < 2 || (argv[1][0] == '-')) {
		fprintf(stderr, usage);
		exit(1);
	}

	/* See if the args request ct.  ION eschews the use of getopt. */
	if(ctArg && strncmp(ctArg, "ct", 3) == 0) {
		custodySwitch = SourceCustodyRequired;
	} else if(defaultDestEid && strncmp(defaultDestEid, "ct", 3) == 0) {
		/* args specify 'ct' but no defaultDestEid. */
		custodySwitch = SourceCustodyRequired;
		defaultDestEid = NULL;
	}


	if(bp_attach() < 0) {
		putErrmsg("Can't bp_attach()", NULL);
		exit(1);
	}

	/* Hook up to ION's private side to get the stats. */
	vdb = getBpVdb();

	if(bp_open(ownEid, &sap) < 0)
	{
		putErrmsg("Can't open own endpoint.", ownEid);
		exit(1);
	}

	sdr = bp_get_sdr();
	signal(SIGINT, handleQuit); 
	signal(SIGUSR1, sendDefault);

	while(needShutdown == 0)
	{
		/* Wait for a bundle. */
		if(bp_receive(sap, &dlv, BP_BLOCKING) < 0)
		{
			bp_close(sap);
			putErrmsg("bpstats2 bundle reception failed.", NULL);
			exit(1);
		}

		if(dlv.result == BpPayloadPresent) {
			sendStats(dlv.bundleSourceEid, theBuffer, sizeof(theBuffer));
			bp_release_delivery(&dlv, 1);
		} else if(dlv.result == BpReceptionInterrupted) {
			if(needSendDefault) {
				needSendDefault = 0;
				sendStats(defaultDestEid, theBuffer, sizeof(theBuffer));
			}
			bp_release_delivery(&dlv, 1);
		} else {
			bp_release_delivery(&dlv, 1);
			break;
		}
	}
	bp_close(sap);
	bp_detach();
	writeErrmsgMemos();
	return 0;
}