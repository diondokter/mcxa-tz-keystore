/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCUXCL_CLNS_ROMAPI_H_
#define _MCUXCL_CLNS_ROMAPI_H_

#include "fsl_common.h"
#include "fsl_glikey.h"
#include "fsl_trng.h"
#include "fsl_crc.h"

/** @brief The PKC RAM secure address */
#define PKC_RAM_ADDR ((uint32_t)0x30002000UL)

/** @brief The PKC RAM size */
#define PKC_RAM_SIZE ((uint32_t)0x1000U)

/**********************************************
 * CL CORE MACRO DEFINITIONS
 **********************************************/

/* Macro to calculate the rounded down number of words that fit into the specified size */
#define MCUXCLCORE_NUM_OF_WORDS_FLOOR(wordsize, size) ((size) / (wordsize))

/* Macro to calculate the rounded up number of words needed to fit an object of the specified size */
#define MCUXCLCORE_NUM_OF_WORDS_CEIL(wordsize, size) (((size) + (wordsize)-1u) / (wordsize))

/* Macro to calculate the rounded down number of CPU words that fit into the specified size */
#define MCUXCLCORE_NUM_OF_CPUWORDS_FLOOR(size) MCUXCLCORE_NUM_OF_WORDS_FLOOR(sizeof(uint32_t), size)

/* Macro to calculate the rounded up number of CPU words needed to fit an object of the specified size */
#define MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(size) MCUXCLCORE_NUM_OF_WORDS_CEIL(sizeof(uint32_t), size)

/* Macro to round up a given size to the nearest multiple of a specified word size */
#define MCUXCLCORE_ALIGN_TO_WORDSIZE(wordsize, size) (MCUXCLCORE_NUM_OF_WORDS_CEIL(wordsize, size) * (wordsize))

/* Macro to round up a given size to the nearest multiple of the CPU word size */
#define MCUXCLCORE_ALIGN_TO_CPU_WORDSIZE(size) MCUXCLCORE_ALIGN_TO_WORDSIZE(sizeof(uint32_t), size)

/* Macro to calculate the maximum of two values */
#define MCUXCLCORE_MAX(a, b) (((a) > (b)) ? (a) : (b))

/* Macro to calculate the minimum of two values */
#define MCUXCLCORE_MIN(a, b) (((a) < (b)) ? (a) : (b))

/**********************************************
 * CL FLOW PROTECTION MACRO DEFINITIONS
 **********************************************/
#define MCUX_CL_ROMAPI_PROTECTED_TYPE(resultType) uint64_t

/**
 * @brief Call the provided ROM API function and get its status
 *
 * A2TS ROM CryptoLib has flow protection enabled. For the ROM API, we only want
 * the status, not the token. This macro strips the flow protection token from
 * retval and leaves the result visible for processing after the function call.
 */
#define MCUX_CL_ROMAPI_FUNCTION_CALL(result, call)            \
        uint32_t result = 0u;                                 \
        do {                                                  \
            const uint64_t result_full = (call);              \
            result = ((result_full << 0u) & (0xFFFFFFFFuLL)); \
        } while(false);

/**********************************************
 * DATA TYPE DEFINITIONS
 **********************************************/

/**
 * The following typedefs define opaque types used throughout the ROM API
 * without providing direct access to their internal structures.
 */

typedef void * const mcuxClSession_Handle_t;
typedef void * const mcuxClKey_Handle_t;
typedef const void * mcuxClKey_Type_t;
typedef const void * const mcuxClKey_Generation_t;
typedef void * mcuxClRandom_Context_t;
typedef const void * mcuxClRandom_Mode_t;
typedef const uint8_t * mcuxCl_InputBuffer_t;
typedef uint8_t * mcuxCl_Buffer_t;
typedef const void * const mcuxClSignature_Mode_t;
typedef const void * const mcuxClHash_Algo_t;
typedef const void * const mcuxClXof_Algo_t;

/**
 * @brief Type for mcuxClSession status codes
 */
typedef uint32_t mcuxClSession_Status_t;

/**
 * @brief Signature status code
 *
 * This type provides information about the status of the Signature operation
 * that has been performed.
 */
typedef uint32_t mcuxClSignature_Status_t;

/**
 * @brief Hash Status type
 *
 * This type is used for hash return values: @ref mcuxClHashStatusValues
 */
typedef uint32_t mcuxClHash_Status_t;

/**
 * @brief Type for status codes of mcuxClRandom component functions.
 *
 * This type provides information about the status of the Random operation
 * that has been performed.
 */
typedef uint32_t mcuxClRandom_Status_t;

/**
 * @brief Type for Mac component error codes.
 */
typedef uint32_t mcuxClMac_Status_t;

/**
 * @brief Type for Key component error codes.
 */
typedef uint32_t mcuxClKey_Status_t;

/**
 * @brief Type for mcuxClResource status codes
 *
 * This type provides information about the status of the Resource operation
 * that has been performed.
 */
typedef uint32_t mcuxClResource_Status_t;

/**
 * @brief Type used to indicate the ML-DSA mode (44, 65, 87), the API mode (PURE and PREHASH).
 * The structure is as follows: RFU (5 nibbles) || API mode (nibble) || ML-DSA mode (1 byte).
 * Explicit internal ML-DSA modes are described in Internal_Constants.h.
 */
typedef uint32_t mcuxClMlDsa_Mode_t;

/**
 * @brief Options type to select different modes of ML-DSA.
 */
typedef uint32_t mcuxClMlDsa_Options_t;

#define MCUXCLSESSION_STATUS_OK             ((mcuxClSession_Status_t)0x0EEE2E03u) ///< Session operation successful
#define MCUXCLSESSION_STATUS_ERROR          ((mcuxClSession_Status_t)0x0EEE5330u) ///< Error occurred during Session operation
#define MCUXCLSESSION_STATUS_HW_UNAVAILABLE ((mcuxClSession_Status_t)0x0EEE5334u) ///< Required HW is unavailable
#define MCUXCLSIGNATURE_STATUS_OK           (0x0FF72E03u)
#define MCUXCLRANDOM_STATUS_OK              (0x0BBB2E03u) ///< Random function returned successfully
#define MCUXCLHASH_STATUS_OK                ((mcuxClHash_Status_t)0x06662E03u) ///< Hash operation successful
#define MCUXCLKEY_STATUS_OK ((mcuxClKey_Status_t)0x07772E03u) ///< Key operation successful
#define MCUXCLRESOURCE_STATUS_OK           ((mcuxClResource_Status_t)0x0CCC2E03u) ///< Resource operation successful
#define MCUXCLRESOURCE_STATUS_UNAVAILABLE  ((mcuxClResource_Status_t)0x0CCC5334u) ///< Resource request failed
#define MCUXCLRESOURCE_STATUS_ERROR        ((mcuxClResource_Status_t)0x0CCC5330u) ///< Error occurred during Resource operation
#define MCUXCLRESOURCE_STATUS_FAULT_ATTACK ((mcuxClResource_Status_t)0x0CCCF0F0u) ///< Fault attack detected

#define SESSION_DESCRIPTOR_SIZE (88) /** Size of struct mcuxClSession_Descriptor */
#define MCUXCLRESOURCE_CONTEXT_SIZE (120u) /** Size (in bytes) of mcuxClResource context */

/**
 * @brief Structure for mcuxClSession Descriptor
 */
struct mcuxClSession_Descriptor
{
    uint8_t data[SESSION_DESCRIPTOR_SIZE]; ///< Opaque data structure for the descriptor
};
typedef struct mcuxClSession_Descriptor mcuxClSession_Descriptor_t;

/* Key sizes */
#define MCUXCLAES_AES128_KEY_SIZE (MCUXCLKEY_SIZE_128) ///< AES-128 key size in bytes
#define MCUXCLAES_AES192_KEY_SIZE (MCUXCLKEY_SIZE_192) ///< AES-192 key size in bytes
#define MCUXCLAES_AES256_KEY_SIZE (MCUXCLKEY_SIZE_256) ///< AES-256 key size in bytes

#define MCUXCLKEY_LOADOPTION_SLOT_COPRO_MASK  (0xc0U)
#define MCUXCLKEY_LOADOPTION_SLOT_COPRO_SHIFT (6U)
#define MCUXCLKEY_LOADOPTION_SLOT_COPRO_SGI   ((uint32_t)0x01U << MCUXCLKEY_LOADOPTION_SLOT_COPRO_SHIFT)

#define MCUXCLKEY_LOADOPTION_SLOT_SLOT_MASK  (0x3fU)
#define MCUXCLKEY_LOADOPTION_SLOT_SLOT_SHIFT (0U)
#define MCUXCLKEY_LOADOPTION_SLOT_SGI_KEY_0 \
    (MCUXCLKEY_LOADOPTION_SLOT_COPRO_SGI |  \
     ((uint32_t)0U << MCUXCLKEY_LOADOPTION_SLOT_SLOT_SHIFT)) ///< SGI key slot 0. Reserved for CL internal key usage.
#define MCUXCLKEY_LOADOPTION_SLOT_SGI_KEY_1 \
    (MCUXCLKEY_LOADOPTION_SLOT_COPRO_SGI |  \
     ((uint32_t)1U << MCUXCLKEY_LOADOPTION_SLOT_SLOT_SHIFT)) ///< SGI key slot 1. Reserved for CL internal key usage.
#define MCUXCLKEY_LOADOPTION_SLOT_SGI_KEY_2 \
    (MCUXCLKEY_LOADOPTION_SLOT_COPRO_SGI | ((uint32_t)2U << MCUXCLKEY_LOADOPTION_SLOT_SLOT_SHIFT)) ///< SGI key slot 2
#define MCUXCLKEY_LOADOPTION_SLOT_SGI_KEY_3 \
    (MCUXCLKEY_LOADOPTION_SLOT_COPRO_SGI | ((uint32_t)3U << MCUXCLKEY_LOADOPTION_SLOT_SLOT_SHIFT)) ///< SGI key slot 3
#define MCUXCLKEY_LOADOPTION_SLOT_SGI_KEY_4 \
    (MCUXCLKEY_LOADOPTION_SLOT_COPRO_SGI | ((uint32_t)4U << MCUXCLKEY_LOADOPTION_SLOT_SLOT_SHIFT)) ///< SGI key slot 4
#define MCUXCLKEY_LOADOPTION_SLOT_SGI_KEY_5 \
    (MCUXCLKEY_LOADOPTION_SLOT_COPRO_SGI | ((uint32_t)5U << MCUXCLKEY_LOADOPTION_SLOT_SLOT_SHIFT)) ///< SGI key slot 5
#define MCUXCLKEY_LOADOPTION_SLOT_SGI_KEY_6 \
    (MCUXCLKEY_LOADOPTION_SLOT_COPRO_SGI | ((uint32_t)6U << MCUXCLKEY_LOADOPTION_SLOT_SLOT_SHIFT)) ///< SGI key slot 6
#define MCUXCLKEY_LOADOPTION_SLOT_SGI_KEY_7 \
    (MCUXCLKEY_LOADOPTION_SLOT_COPRO_SGI | ((uint32_t)7U << MCUXCLKEY_LOADOPTION_SLOT_SLOT_SHIFT)) ///< SGI key slot 7

/**********************************************
 * STRUCTURES
 **********************************************/

#define MCUXCLKEY_DESCRIPTOR_SIZE          (64u)
#define MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS (MCUXCLKEY_DESCRIPTOR_SIZE / sizeof(uint32_t))

#define MCUXCLECC_WEIERECC_SECP384R1_SIZE_PRIMEP                  (48U)
#define MCUXCLECC_WEIERECC_SECP384R1_SIZE_BASEPOINTORDER          (48U)
#define MCUXCLECC_WEIERECC_SECP384R1_SIZE_PRIVATEKEY              (MCUXCLECC_WEIERECC_SECP384R1_SIZE_BASEPOINTORDER)
#define MCUXCLECC_WEIERECC_SECP384R1_SIZE_PUBLICKEY               (2U * MCUXCLECC_WEIERECC_SECP384R1_SIZE_PRIMEP)
#define MCUXCLECC_WEIERECC_SECP384R1_SIZE_SHAREDSECRET            (MCUXCLECC_WEIERECC_SECP384R1_SIZE_PRIMEP)
#define MCUXCLECC_WEIERECC_SECP384R1_SIZE_SIGNATURE               (2U * MCUXCLECC_WEIERECC_SECP384R1_SIZE_BASEPOINTORDER)
#define MCUXCLECC_WEIERECC_SECP384R1_SIZE_ENCPOINT_SEC_NEUTRAL    (1u)
#define MCUXCLECC_WEIERECC_SECP384R1_SIZE_ENCPOINT_SEC_COMPRESSED (1u + MCUXCLECC_WEIERECC_SECP384R1_SIZE_PRIMEP)
#define MCUXCLECC_WEIERECC_SECP384R1_SIZE_ENCPOINT_SEC_UNCOMPRESSED \
    (1u + (2u * MCUXCLECC_WEIERECC_SECP384R1_SIZE_PRIMEP))

#define MCUXCLECC_WEIERECC_NIST_P384_SIZE_PRIMEP               (MCUXCLECC_WEIERECC_SECP384R1_SIZE_PRIMEP)
#define MCUXCLECC_WEIERECC_NIST_P384_SIZE_BASEPOINTORDER       (MCUXCLECC_WEIERECC_SECP384R1_SIZE_BASEPOINTORDER)
#define MCUXCLECC_WEIERECC_NIST_P384_SIZE_PRIVATEKEY           (MCUXCLECC_WEIERECC_SECP384R1_SIZE_PRIVATEKEY)
#define MCUXCLECC_WEIERECC_NIST_P384_SIZE_PUBLICKEY            (MCUXCLECC_WEIERECC_SECP384R1_SIZE_PUBLICKEY)
#define MCUXCLECC_WEIERECC_NIST_P384_SIZE_SHAREDSECRET         (MCUXCLECC_WEIERECC_SECP384R1_SIZE_SHAREDSECRET)
#define MCUXCLECC_WEIERECC_NIST_P384_SIZE_SIGNATURE            (MCUXCLECC_WEIERECC_SECP384R1_SIZE_SIGNATURE)
#define MCUXCLECC_WEIERECC_NIST_P384_SIZE_ENCPOINT_SEC_NEUTRAL (MCUXCLECC_WEIERECC_SECP384R1_SIZE_ENCPOINT_SEC_NEUTRAL)
#define MCUXCLECC_WEIERECC_NIST_P384_SIZE_ENCPOINT_SEC_COMPRESSED \
    (MCUXCLECC_WEIERECC_SECP384R1_SIZE_ENCPOINT_SEC_COMPRESSED)
#define MCUXCLECC_WEIERECC_NIST_P384_SIZE_ENCPOINT_SEC_UNCOMPRESSED \
    (MCUXCLECC_WEIERECC_SECP384R1_SIZE_ENCPOINT_SEC_UNCOMPRESSED)

#define MCUXCLRANDOMMODES_CTR_DRBG_AES256_CONTEXT_SIZE (72u)
#define MCUXCLRANDOMMODES_CTR_DRBG_AES256_CONTEXT_SIZE_IN_WORDS \
    ((MCUXCLRANDOMMODES_CTR_DRBG_AES256_CONTEXT_SIZE + sizeof(uint32_t) - 1u) / sizeof(uint32_t))

/******************* DRBG **************************/

static const void * mcuxClRandomModes_Mode_CtrDrbg_AES256_DRG3 = (void *)0x1100be7c;// &mcuxClRandomModes_mdCtrDrbg_AES256_DRG3;

/******************* Buffer ************************/
#define MCUXCLBUFFER_INIT(name, info, ptr, size) MCUXCLBUFFER_INIT_RW(name, info, ptr, size)

#define MCUXCLBUFFER_INIT_RW(name, info, ptr, size) MCUXCLBUFFER_INIT_PLAIN_RW_IMPL(name, info, ptr, size)

#define MCUXCLBUFFER_INIT_PLAIN_RW_IMPL(_name, _info, _ptr, _size) mcuxCl_Buffer_t(_name) = (mcuxCl_Buffer_t)(_ptr)

#define MCUXCLBUFFER_INIT_RO(name, info, ptr, size) MCUXCLBUFFER_INIT_PLAIN_RO_IMPL(name, info, ptr, size)

#define MCUXCLBUFFER_INIT_PLAIN_RO_IMPL(_name, _info, _ptr, _size) \
    mcuxCl_InputBuffer_t(_name) = (mcuxCl_InputBuffer_t)(_ptr)

/******************* ECDSA *************************/

/**
 * @brief Key type pointer for private ECC keys for Weierstrass curve NIST P-384.
 */
static const mcuxClKey_Type_t mcuxClKey_Type_WeierECC_NIST_P384_Priv = (mcuxClKey_Type_t)0x110072a0;

/**
 * @brief Key type pointer for public ECC Weierstrass keys for Weierstrass curve NIST P-384.
 *
 * @note This key type is used for ECDSA signature verification operations ONLY.
 */
static const mcuxClKey_Type_t mcuxClKey_Type_WeierECC_NIST_P384_Pub_For_Verify = (mcuxClKey_Type_t)0x1303fe64;

/**
 * @brief Key type pointer for public ECC Weierstrass keys for Weierstrass curve NIST P-384.
 *
 * @note This key type is used for ECC key generation operations ONLY.
 */
static const mcuxClKey_Type_t mcuxClKey_Type_WeierECC_NIST_P384_Pub_For_Keygen = (mcuxClKey_Type_t)0x11007ec0;

/**
 * @brief Opaque ECDSA mode descriptor. Cannot be pulled from ROM/IFR.
 */
const uint32_t mcuxClSignature_ModeDescriptor_ECDSA[] = {
    0x1100c629, /* mcuxClEcc_ECDSA_GenerateSignature */
    0x3CCA,     /* MCUX_CSSL_FP_FUNCID_mcuxClEcc_ECDSA_GenerateSignature */
    0x130314d5, /* mcuxClEcc_ECDSA_VerifySignature */
    0x5574,     /* MCUX_CSSL_FP_FUNCID_mcuxClEcc_ECDSA_VerifySignature */
    0x11008008  /* mcuxClEcc_ECDSA_ProtocolDescriptor */
};

/**
 * @brief ECDSA Signature mode
 */
static mcuxClSignature_Mode_t mcuxClSignature_Mode_ECDSA = (mcuxClSignature_Mode_t)mcuxClSignature_ModeDescriptor_ECDSA;

/**
 * @brief ECDSA Key generation algorithm
 */
static mcuxClKey_Generation_t mcuxClKey_Generation_ECDSA = (mcuxClKey_Generation_t)0x11007204; //&mcuxClKey_GenerationDescriptor_ECDSA;

/******************* Hash **************************/

#define MCUXCLHASH_OUTPUT_SIZE_SHA_384 (48U) ///< SHA-384 output size: 384 bit (48 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA_512 (64U) ///< SHA-512 output size: 512 bit (64 bytes)

/**
 * @brief Sha-384 algorithm descriptor
 *        Sha-384 hash calculation using the Hash functionality SGI.
 * @note Interrupts must be enabled on the DMA input channel with a properly installed handler.
 * If the non-blocking Hash operation returns @ref MCUXCLHASH_STATUS_JOB_STARTED, a
 * non-blocking operation has started and the CPU is unblocked in the meantime.
 */
#define mcuxClHash_Algorithm_Sha384 ((void *)0x1303f298) /* mcuxClHash_AlgorithmDescriptor_Sgi_Sha384 */

/**
 * @brief Sha-512 algorithm descriptor
 *        Sha-512 hash calculation using the Hash functionality SGI.
 * @note Interrupts must be enabled on the DMA input channel with a properly installed handler.
 * If the non-blocking Hash operation returns @ref MCUXCLHASH_STATUS_JOB_STARTED, a
 * non-blocking operation has started and the CPU is unblocked in the meantime.
 */
#define mcuxClHash_Algorithm_Sha512 ((void *)0x1303f2c8) /* mcuxClHash_AlgorithmDescriptor_Sgi_Sha512 */


/**
 * @brief SHAKE-128 algorithm descriptor
 *        SHAKE-128 Xof calculation using an underlying software implementation of Keccak.
 */
static mcuxClXof_Algo_t mcuxClXof_Algorithm_Shake_128 = (void *)0x1100fed0; //&mcuxClXof_AlgorithmDescriptor_C_Shake_128;

/**
 * @brief SHAKE-256 algorithm descriptor
 *        SHAKE-256 Xof calculation using an underlying software implementation of Keccak.
 */
static mcuxClXof_Algo_t mcuxClXof_Algorithm_Shake_256 = (void *)0x1100fefc; //&mcuxClXof_AlgorithmDescriptor_C_Shake_256;

/******************* MLDSA *************************/

#define MCUXCLMLDSA_MLDSA87_SK_LEN      (4896U) /** Secret key length for ML-DSA-87. */
#define MCUXCLMLDSA_MLDSA87_PK_LEN      (2592U) /** Public key length for ML-DSA-87. */
#define MCUXCLMLDSA_MLDSA87_SIG_LEN     (4627U) /** Signature length for ML-DSA-87. */
#define MCUXCLMLDSA_MLDSA87_CTILDE_SIZE (64U)

#define MCUXCLMLDSA_POS_MODE    (0U) /** Position within word of the choice between parameter set 44, 65 or 87 */
#define MCUXCLMLDSA_POS_APIMODE (8U) /** Position within word of the choice between pure and pre-hash mode */

#define MCUXCLMLDSA_MODE_87 ((uint32_t)87U << MCUXCLMLDSA_POS_MODE) /** ML-DSA-87 */
#define MCUXCLMLDSA_MODE_PURE \
    ((uint32_t)0x0U << MCUXCLMLDSA_POS_APIMODE) /** Mode using the pure functionality in the external API */
#define MCUXCLMLDSA_MODE_PREHASH \
    ((uint32_t)0xfU << MCUXCLMLDSA_POS_APIMODE) /** Mode using the pre-hash functionality in the external API */

#define MCUXCLSIGNATURE_MODE_SIZE                     (20u)
#define MCUXCLMLDSA_SIGNATURE_PROTOCOLDESCRIPTOR_SIZE (16u)

/**
 * @brief Key type pointer for public ML-DSA-87 keys.
 */
static const mcuxClKey_Type_t mcuxClKey_Type_MlDsa87_Public =
    (mcuxClKey_Type_t)0x11007040; // &mcuxClKey_TypeDescriptor_MlDsa87_Public

/**
 * @brief Key type pointer for private ML-DSA-87 keys.
 */
static const mcuxClKey_Type_t mcuxClKey_Type_MlDsa87_Private =
    (mcuxClKey_Type_t)0x1100bebc; // &mcuxClKey_TypeDescriptor_MlDsa87_Private

/**
 * @brief ML-DSA-87 Key generation algorithm
 */
static mcuxClKey_Generation_t mcuxClKey_keyGeneration_MlDsa87 =
    (mcuxClKey_Generation_t)0x110072b0; // &mcuxClKey_GenerationDescriptor_MlDsa87

/******************* CLNS API's ********************/
MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClSession_Status_t) (*mcuxClSession_init)(mcuxClSession_Handle_t pSession,
                                             uint32_t *const pCpuWaBuffer,
                                             uint32_t cpuWaLength,
                                             uint32_t *const pPkcWaBuffer,
                                             uint32_t pkcWaLength) =
    (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClSession_Status_t)(*)(mcuxClSession_Handle_t, uint32_t *, uint32_t, uint32_t *, uint32_t))0x1301ff69;

MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClSession_Status_t) (*mcuxClSession_cleanup)(mcuxClSession_Handle_t pSession) =
    (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClSession_Status_t)(*)(mcuxClSession_Handle_t))0x13020031;

MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClKey_Status_t) (*mcuxClKey_init)(mcuxClSession_Handle_t pSession,
                                     mcuxClKey_Handle_t key,
                                     mcuxClKey_Type_t type,
                                     const uint8_t *pKeyData,
                                     uint32_t keyDataLength) = (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClKey_Status_t)(*)(mcuxClSession_Handle_t,
                                                                                      mcuxClKey_Handle_t,
                                                                                      mcuxClKey_Type_t,
                                                                                      const uint8_t *,
                                                                                      uint32_t))0x130217c5;

MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClKey_Status_t) (*mcuxClKey_generate_keypair)(mcuxClSession_Handle_t pSession,
                                                 mcuxClKey_Generation_t generation,
                                                 mcuxClKey_Handle_t privKey,
                                                 mcuxClKey_Handle_t pubKey) =
    (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClKey_Status_t)(*)(
        mcuxClSession_Handle_t, mcuxClKey_Generation_t, mcuxClKey_Handle_t, mcuxClKey_Handle_t))0x1100a455;

MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClRandom_Status_t) (*mcuxClRandom_init)(mcuxClSession_Handle_t pSession,
                                           mcuxClRandom_Context_t pContext,
                                           mcuxClRandom_Mode_t mode) =
    (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClRandom_Status_t)(*)(mcuxClSession_Handle_t, mcuxClRandom_Context_t, mcuxClRandom_Mode_t))0x110077ed;

MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClRandom_Status_t) (*mcuxClRandom_ncInit)(mcuxClSession_Handle_t pSession) =
    (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClRandom_Status_t)(*)(mcuxClSession_Handle_t))0x1303ce81;

MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClRandom_Status_t) (*mcuxClRandom_reseed)(mcuxClSession_Handle_t pSession) =
    (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClRandom_Status_t)(*)(mcuxClSession_Handle_t))0x1100eda9;

MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClRandom_Status_t) (*mcuxClRandom_generate)(mcuxClSession_Handle_t pSession,
                                               mcuxCl_Buffer_t pOut,
                                               uint32_t outLength) = (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClRandom_Status_t)(*)(mcuxClSession_Handle_t,
                                                                                               mcuxCl_Buffer_t,
                                                                                               uint32_t))0x1100797d;

MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClRandom_Status_t) (*mcuxClRandom_uninit)(mcuxClSession_Handle_t pSession) =
    (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClRandom_Status_t)(*)(mcuxClSession_Handle_t))0x1100b521;

MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClSignature_Status_t) (*mcuxClSignature_sign)(mcuxClSession_Handle_t pSession,
                                                 mcuxClKey_Handle_t key,
                                                 mcuxClSignature_Mode_t mode,
                                                 mcuxCl_InputBuffer_t pIn,
                                                 uint32_t inSize,
                                                 mcuxCl_Buffer_t pSignature,
                                                 uint32_t *const pSignatureSize) =
    (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClSignature_Status_t)(*)(mcuxClSession_Handle_t,
                                 mcuxClKey_Handle_t,
                                 mcuxClSignature_Mode_t,
                                 mcuxCl_InputBuffer_t,
                                 uint32_t,
                                 mcuxCl_Buffer_t,
                                 uint32_t *))0x1100a5af;

MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClSignature_Status_t) (*mcuxClSignature_verify)(mcuxClSession_Handle_t pSession,
                                                   mcuxClKey_Handle_t key,
                                                   mcuxClSignature_Mode_t mode,
                                                   mcuxCl_InputBuffer_t pIn,
                                                   uint32_t inSize,
                                                   mcuxCl_InputBuffer_t pSignature,
                                                   uint32_t signatureSize) =
    (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClSignature_Status_t)(*)(
        mcuxClSession_Handle_t,
        mcuxClKey_Handle_t,
        mcuxClSignature_Mode_t,
        mcuxCl_InputBuffer_t,
        uint32_t,
        mcuxCl_InputBuffer_t,
        uint32_t))0x1303b031;

/**
 * @brief Mode constructor for ML-DSA signature generation and verification
 *
 * @param[out]      pSignatureMode        Pointer to a mode descriptor that will be updated by this function.
 * @param[in,out]   pProtocolDescriptor   Pointer to an ML-DSA protocol descriptor that will be updated by this function.
 * @param[in]       options               Struct type used to indicate the ML-DSA mode (44, 65, 87), the API mode (PURE and PRE-HASH) and whether internal or external API is expected.
 * @param[in]       pPreHashAlgo          If PRE-HASH is used, the algorithm that is used to pre-hash the message; otherwise NULL.
 *                                        Note that this is a void pointer: it will either accept mcuxClHash_Algo_t or mcuxClXof_Algo_t.
 * @param[in]       ctx                   User provided context string; otherwise NULL.
 * @param[in]       ctxLength             Length of the user provided context string; otherwise 0U.
 */
MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClSignature_Status_t) (*mcuxClMlDsa_SignatureModeConstructor)(
    void *pSignatureMode,
    void *pProtocolDescriptor,
    mcuxClMlDsa_Options_t options,
    const void *const pPreHashAlgo,
    mcuxCl_InputBuffer_t ctx,
    uint8_t ctxLength) = (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClSignature_Status_t)(*)(void *,
                                                      void *,
                                                      mcuxClMlDsa_Options_t,
                                                      const void *const,
                                                      mcuxCl_InputBuffer_t,
                                                      uint8_t))0x1100a4ef;

MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClResource_Status_t) (*mcuxClResource_init)(void *pResourceCtx,
                                void *pMutexAcquire,
                                void *pMutexRelease) =
    (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClResource_Status_t)(*)(void *,
                 void *,
                 void *))0x130201d5;

MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClSession_Status_t) (*mcuxClSession_setResource)(mcuxClSession_Handle_t pSession, void *pResourceCtx) =
    (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClSession_Status_t)(*)(mcuxClSession_Handle_t, void *))0x1301fffd;


MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClHash_Status_t) (*mcuxClHash_compute)(mcuxClSession_Handle_t pSession,
                                          mcuxClHash_Algo_t algorithm,
                                          mcuxCl_InputBuffer_t pIn,
                                          uint32_t inSize,
                                          mcuxCl_Buffer_t pOut,
                                          uint32_t *const pOutSize) =
    (MCUX_CL_ROMAPI_PROTECTED_TYPE(mcuxClHash_Status_t)(*)(mcuxClSession_Handle_t,
                            mcuxClHash_Algo_t,
                            mcuxCl_InputBuffer_t,
                            uint32_t,
                            mcuxCl_Buffer_t,
                            uint32_t *const))0x130203dd;

/**********************************************
 * REQUIRED INITIALIZATION ROUTINES
 **********************************************/

/*!
 * @brief SGI Init after power down.
 *
 * This function enable all SGI related clocks, enable SGI and enables other needed HW used by SGI crypto library.
 *
 * @param base SGI peripheral address.
 *
 * @return kStatus_Success upon success, kStatus_Fail otherwise
 */
static status_t SGI_PowerDownWakeupInit(SGI_Type *base)
{
    status_t status = kStatus_Fail;

    (void)base; // Unused

    /* Release reset of TRNG */
    RESET_ReleasePeripheralReset(kTRNG0_RST_SHIFT_RSTn);

    /* Enable SGI and TRNG clock */
    CLOCK_EnableClock(kCLOCK_GateSGI0);
    CLOCK_EnableClock(kCLOCK_GateTRNG0);

    /* Initialiaze TRNG which is used by SGI PKC crypto library */
    trng_config_t trngcon;
    status = TRNG_GetDefaultConfig(&trngcon);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = TRNG_Init(TRNG0, &trngcon);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Initialiaze CRC which is used by SGI PKC crypto library */
    crc_config_t config;
    CRC_GetDefaultConfig(&config);
    CRC_Init(CRC0, &config);

    return status;
}


static status_t GlikeyWriteEnable(GLIKEY_Type *base, uint8_t idx)
{
    if (kStatus_Success != GLIKEY_StartEnable(base, idx))
    {
        return kStatus_Fail;
    }
    if (kStatus_Success != GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP1))
    {
        return kStatus_Fail;
    }
    if (kStatus_Success != GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP2))
    {
        return kStatus_Fail;
    }
    if (kStatus_Success != GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP3))
    {
        return kStatus_Fail;
    }
    if (kStatus_Success != GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP_EN))
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}

/*!
 * @brief PKC Init after power down.
 *
 * This function enables RAM interleave, RAM execute permission, clocks and reset PKC peripheral.
 *
 * @param base PKC peripheral address.
 *
 * @return kStatus_Success upon success, kStatus_Fail otherwise
 */
static status_t PKC_InitNoZeroize(PKC_Type *base)
{
    status_t status = kStatus_Fail;
    /* Turn on PKC RAM inteleaving. This setting is need for PKC L0 memory access. */
#if defined(SYSCON_RAM_INTERLEAVE_INTERLEAVE_MASK) && SYSCON_RAM_INTERLEAVE_INTERLEAVE_MASK
    SYSCON->RAM_INTERLEAVE |= SYSCON_RAM_INTERLEAVE_INTERLEAVE_MASK;
#endif

#if defined(SYSCON_RAM_CASP_CTRL_INTERLEAVE_MASK) && SYSCON_RAM_CASP_CTRL_INTERLEAVE_MASK
    SYSCON->RAM_CASP_CTRL |= SYSCON_RAM_CASP_CTRL_INTERLEAVE_MASK;
#endif

    /* Reset PKC */
    RESET_ReleasePeripheralReset(kPKC0_RST_SHIFT_RSTn);
    /* Enable PKC clock */
    CLOCK_EnableClock(kCLOCK_GatePKC0);

    /* By default SRAM have R/W enabled, execute permission is disabled */
    /* Enable execute permission in RAM atleast where stack is located (RAM C3 0x20038000 - 0x2003BFF) by setting
     * SRAM_XEN register in SYSCON */
    /* This is needed so PKC can fetch FUP program */
#if defined(SYSCON_SRAM_XEN_RAMC_XEN_MASK) && SYSCON_SRAM_XEN_RAMC_XEN_MASK
    SYSCON->SRAM_XEN |= SYSCON_SRAM_XEN_RAMC_XEN_MASK;
#endif

#if defined(SECCON_RAM_XEN_RAMA1_XEN_MASK) && SECCON_RAM_XEN_RAMA1_XEN_MASK
    SECCON->RAM_XEN |= SECCON_RAM_XEN_RAMA1_XEN_MASK;
#endif

    /* SRAM_XEN is duplicated by SRAM_XEN_DP which is protected by Glikey, SRAM_XEN_DP is protected by Glikey, enable
     * write, and write same value as in SRAM_XEN register */
    status = GLIKEY_SyncReset(GLIKEY0);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Move Glikey FSM to write enable */
    status = GlikeyWriteEnable(GLIKEY0, 2u);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Write value into SRAM_XEN_DP */
#if defined(SYSCON_SRAM_XEN_RAMC_XEN_MASK) && SYSCON_SRAM_XEN_RAMC_XEN_MASK
    SYSCON->SRAM_XEN_DP |= SYSCON_SRAM_XEN_DP_RAMC_XEN_MASK;
#endif

#if defined(SECCON_RAM_XEN_RAMA1_XEN_MASK) && SECCON_RAM_XEN_RAMA1_XEN_MASK
    SECCON->RAM_XEN_DP |= SECCON_RAM_XEN_RAMA1_XEN_MASK;
#endif

    /* End of write enable */
    status = GLIKEY_EndOperation(GLIKEY0);
    if (status != kStatus_Success)
    {
        return status;
    }

    return kStatus_Success;
}

/*!
 * @brief PKC Init after power down.
 *
 * This function enables RAM interleave, RAM execute permission, clocks, zeroize the PKC RAM and reset PKC peripheral.
 *
 * @param base PKC peripheral address.
 *
 * @return kStatus_Success upon success, kStatus_Fail otherwise
 */
static status_t PKC_PowerDownWakeupInit(PKC_Type *base)
{
    status_t status = kStatus_Fail;

    (void)base; // Unused

    status = PKC_InitNoZeroize(PKC0);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Zeroize the PKC RAM */
    for (uint32_t i = 0; i < PKC_RAM_SIZE / sizeof(uint32_t); i++)
    {
        ((uint32_t *)PKC_RAM_ADDR)[i] = 0x0;
    }

    return kStatus_Success;
}

#endif /* _MCUXCL_CLNS_ROMAPI_H_ */
