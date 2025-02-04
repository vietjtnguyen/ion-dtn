/******************************************************************************
 **                           COPYRIGHT NOTICE
 **      (c) 2022 The Johns Hopkins University Applied Physics Laboratory
 **                         All rights reserved.
 ******************************************************************************/


/*****************************************************************************
 **
 ** File Name: bib_hmac_sha2.h
 **
 ** Namespace:
 **    bpsec_bhssci_   SCI Interface functions
 **    bpsec_bhsscutl  General utilities
 **
 ** Description:
 **
 **     This file implements The BIB-HMAC-SHA2 security context standardized
 **     by RFC9173.
 **
 ** Notes:
 **
 **
 ** Assumptions:
 **
 ** Modification History:
 **  MM/DD/YY  AUTHOR         DESCRIPTION
 **  --------  ------------   ---------------------------------------------
 **  03/07/22  E. Birrane     Initial implementation
 *****************************************************************************/

#ifndef _BPSEC_BHSSC_H_
#define _BPSEC_BHSSC_H_

#include "csi.h"
#include "sci.h"
#include "sc_util.h"
#include "bpsec_util.h"
#include "sci_valmap.h"


#define BPSEC_BIB_HMAC_SHA2_SC_ID 1

/*
 * SHA Variant - Options and Default Value
 * https://www.rfc-editor.org/rfc/rfc9173.html#name-sha-variant
 */

#define BPSEC_BHSSC_SV_256 (0x5)
#define BPSEC_BHSSC_SV_384 (0x6)
#define BPSEC_BHSSC_SV_512 (0x7)

//#define BPSEC_BHSSC_SV_DEFAULT BPSEC_BHSSC_SV_384
#define BPSEC_BHSSC_SV_DEFAULT BPSEC_BHSSC_SV_256


/**
 * Security Context Identifiers
 *
 * BIB-HMAC-SHA2 defines three parameters and one result type that
 * can be generated by the context and used to populate security
 * blocks.
 *
 * This enumeration lists these items.
 *
 */
typedef enum 
{
    BPSEC_BHSSC_PARM_LTK_NAME    = 4, /** Parm.   The LTK key name to use    */
	BPSEC_BHSSC_PARM_SHA_VAR_ID  = 1, /** Parm.   The SHA Variant Used.      */
	BPSEC_BHSSC_PARM_WRAPPED_KEY = 2, /** Parm.   The AES-Wrapped key.       */
	BPSEC_BHSSC_PARM_SCOPE_FLAGS = 3, /** Parm.   The integrity Scope Flags. */
    BPSEC_BHSSC_RESULT_EHMAC     = 1  /** Result. The Expected HMAC.         */
} sc_bhs_ids_t;



/*****************************************************************************
 *                           SC Interface Functions                          *
 *****************************************************************************/



/* Generic Security Context Interface Functions*/

int           bpsec_bhssci_procInBlk(sc_state *state, AcqWorkArea *wk, BpsecInboundASB *asb, LystElt tgtBlkElt, BpsecInboundTargetResult *tgtResult);
int           bpsec_bhssci_procOutBlk(sc_state *state, Lyst extraParms, Bundle *bundle, BpsecOutboundASB *asb, BpsecOutboundTargetResult *tgtResult);
sc_value_map* bpsec_bhssci_valMapGet();


/* Generic Utilities */
uint8_t* bpsec_bhsscutl_computeSignature(BpsecSerializeData preamble, Object zcoObj, int zcoLen, int csi_suite, csi_val_t csi_key, csi_svcid_t svc);




#endif
