#[repr(transparent)]
pub struct McuxClSessionDescriptor([u32; 23]);
pub type McuxClSessionHandle = *const McuxClSessionDescriptor;

type McuxClKeyType = *const core::ffi::c_void;
/**
 * @brief Key type pointer for public ML-DSA-87 keys.
 */
const MCUX_CL_KEY_TYPE_ML_DSA87_PUBLIC: McuxClKeyType = 0x11007040 as McuxClKeyType;
/**
 * @brief Key type pointer for private ML-DSA-87 keys.
 */
const MCUX_CL_KEY_TYPE_ML_DSA87_PRIVATE: McuxClKeyType = 0x1100bebc as _;
/**
 * @brief Key type pointer for private ECC keys for Weierstrass curve NIST P-384.
 */
const MCUX_CL_KEY_TYPE_WEIER_ECC_NIST_P384_PRIV: McuxClKeyType = 0x110072a0 as _;
/**
 * @brief Key type pointer for public ECC Weierstrass keys for Weierstrass curve NIST P-384.
 *
 * @note This key type is used for ECDSA signature verification operations ONLY.
 */
const MCUX_CL_KEY_TYPE_WEIER_ECC_NIST_P384_PUB_FOR_VERIFY: McuxClKeyType = 0x1303fe64 as _;
/**
 * @brief Key type pointer for public ECC Weierstrass keys for Weierstrass curve NIST P-384.
 *
 * @note This key type is used for ECC key generation operations ONLY.
 */
const MCUX_CL_KEY_TYPE_WEIER_ECC_NIST_P384_PUB_FOR_KEYGEN: McuxClKeyType = 0x11007ec0 as _;

type McuxKeyGeneration = *const core::ffi::c_void;
/**
 * @brief ECDSA Key generation algorithm
 */
const MCUX_CL_KEY_GENERATION_ECDSA: McuxKeyGeneration = 0x11007204 as _;
/**
 * @brief ML-DSA-87 Key generation algorithm
 */
const MCUX_CL_KEY_KEY_GENERATION_ML_DSA87: McuxKeyGeneration = 0x110072b0 as _;

type McuxClSignatureMode = *const core::ffi::c_void;
/**
 * @brief Opaque ECDSA mode descriptor. Cannot be pulled from ROM/IFR.
 */
static MCUX_CL_SIGNATURE_MODE_DESCRIPTOR_ECDSA: [u32; 5] = [
    0x1100c629, /* mcuxClEcc_ECDSA_GenerateSignature */
    0x3CCA,     /* MCUX_CSSL_FP_FUNCID_mcuxClEcc_ECDSA_GenerateSignature */
    0x130314d5, /* mcuxClEcc_ECDSA_VerifySignature */
    0x5574,     /* MCUX_CSSL_FP_FUNCID_mcuxClEcc_ECDSA_VerifySignature */
    0x11008008, /* mcuxClEcc_ECDSA_ProtocolDescriptor */
];
/**
 * @brief ECDSA Signature mode
 */
const MCUX_CL_SIGNATURE_MODE_ECDSA: McuxClSignatureMode =
    (&raw const MCUX_CL_SIGNATURE_MODE_DESCRIPTOR_ECDSA) as McuxClSignatureMode;

#[repr(transparent)]
pub struct McuxClKeyDescriptor([u32; 13]);
pub type McuxClKeyHandle = *const McuxClKeyDescriptor;

#[repr(transparent)]
pub struct McuxClRandomDescriptor([u32; 1]);
pub type McuxClRandomContext = *mut McuxClRandomDescriptor;

#[repr(transparent)]
pub struct McuxClRandomModeDescriptor([u32; 5]);
pub type McuxClRandomMode = *const McuxClRandomModeDescriptor;

#[repr(transparent)]
pub struct SessionStatus(u64);
#[repr(transparent)]
pub struct KeyStatus(u64);
#[repr(transparent)]
pub struct RandomStatus(u64);
#[repr(transparent)]
pub struct SignatureStatus(u64);

pub unsafe fn mcux_cl_session_init(
    session: McuxClSessionHandle,
    cpu_workarea_buffer: *const u32,
    cpu_workarea_length: u32,
    pkc_workarea_buffer: *const u32,
    pkc_workarea_length: u32,
) -> SessionStatus {
    unsafe {
        core::mem::transmute::<
            *const (),
            fn(McuxClSessionHandle, *const u32, u32, *const u32, u32) -> SessionStatus,
        >(0x1301ff69 as _)(
            session,
            cpu_workarea_buffer,
            cpu_workarea_length,
            pkc_workarea_buffer,
            pkc_workarea_length,
        )
    }
}

pub unsafe fn mcux_cl_session_cleanup(session: McuxClSessionHandle) -> SessionStatus {
    unsafe {
        core::mem::transmute::<*const (), fn(McuxClSessionHandle) -> SessionStatus>(0x13020031 as _)(
            session,
        )
    }
}

pub unsafe fn mcux_cl_key_init(
    session: McuxClSessionHandle,
    key: McuxClKeyHandle,
    key_type: McuxClKeyType,
    key_data: *const u8,
    key_data_length: u32,
) -> KeyStatus {
    unsafe {
        core::mem::transmute::<
            *const (),
            fn(McuxClSessionHandle, McuxClKeyHandle, McuxClKeyType, *const u8, u32) -> KeyStatus,
        >(0x130217c5 as _)(session, key, key_type, key_data, key_data_length)
    }
}

pub unsafe fn mcux_cl_key_generate_keypair(
    session: McuxClSessionHandle,
    generation: McuxKeyGeneration,
    priv_key: McuxClKeyHandle,
    pub_key: McuxClKeyHandle,
) -> KeyStatus {
    unsafe {
        core::mem::transmute::<
            *const (),
            fn(
                McuxClSessionHandle,
                McuxKeyGeneration,
                McuxClKeyHandle,
                McuxClKeyHandle,
            ) -> KeyStatus,
        >(0x130217c5 as _)(session, generation, priv_key, pub_key)
    }
}

pub unsafe fn mcux_cl_random_init(
    session: McuxClSessionHandle,
    context: McuxClRandomContext,
    mode: McuxClRandomMode,
) -> RandomStatus {
    unsafe {
        core::mem::transmute::<
            *const (),
            fn(McuxClSessionHandle, McuxClRandomContext, McuxClRandomMode) -> RandomStatus,
        >(0x110077ed as _)(session, context, mode)
    }
}

pub unsafe fn mcux_cl_random_nc_init(session: McuxClSessionHandle) -> RandomStatus {
    unsafe {
        core::mem::transmute::<*const (), fn(McuxClSessionHandle) -> RandomStatus>(0x1303ce81 as _)(
            session,
        )
    }
}

pub unsafe fn mcux_cl_random_reseed(session: McuxClSessionHandle) -> RandomStatus {
    unsafe {
        core::mem::transmute::<*const (), fn(McuxClSessionHandle) -> RandomStatus>(0x1100eda9 as _)(
            session,
        )
    }
}

pub unsafe fn mcux_cl_random_generate(
    session: McuxClSessionHandle,
    buffer: *mut u8,
    len: u32,
) -> RandomStatus {
    unsafe {
        core::mem::transmute::<*const (), fn(McuxClSessionHandle, *mut u8, u32) -> RandomStatus>(
            0x1100797d as _,
        )(session, buffer, len)
    }
}

pub unsafe fn mcux_cl_random_uninit(session: McuxClSessionHandle) -> RandomStatus {
    unsafe {
        core::mem::transmute::<*const (), fn(McuxClSessionHandle) -> RandomStatus>(0x1100b521 as _)(
            session,
        )
    }
}

/**
 * @param[in]  pSession                  Pointer to #mcuxClSession_Descriptor
 * @param[in]  key                       Key handle for the input key (word-aligned)
 * @param[in]  mode                      Signature mode descriptor
 * @param[in]  pIn                       Pointer to buffer, which contains the message digest to be signed
 * @param[in]  inSize                    Size of the message digest to be signed
 * @param[out] pSignature                Pointer to buffer, which contains the result (signature)
 * @param[out] pSignatureSize            Will be set to the number of bytes of data that have been written to the pSignature buffer
 *
 * @return A code-flow protected error code (see @ref MCUXCLSIGNATURE_STATUS_)
 * @retval #MCUXCLSIGNATURE_STATUS_OK     if signature is generated successfully;
 *
 * @attention This function uses DRBG and PRNG. Caller needs to check if DRBG and PRNG are ready.
 */
pub unsafe fn mcux_cl_signature_sign(
    session: McuxClSessionHandle,
    key: McuxClKeyHandle,
    mode: McuxClSignatureMode,
    input: *const u8,
    input_size: u32,
    signature: *mut u8,
    signature_size: *mut u32,
) -> SignatureStatus {
    unsafe {
        core::mem::transmute::<
            *const (),
            fn(
                McuxClSessionHandle,
                McuxClKeyHandle,
                McuxClSignatureMode,
                *const u8,
                u32,
                *mut u8,
                *mut u32,
            ) -> SignatureStatus,
        >(0x1100a5af as _)(
            session,
            key,
            mode,
            input,
            input_size,
            signature,
            signature_size,
        )
    }
}

pub unsafe fn mcux_cl_signature_verify(
    session: McuxClSessionHandle,
    key: McuxClKeyHandle,
    mode: McuxClSignatureMode,
    input: *const u8,
    input_size: u32,
    signature: *const u8,
    signature_size: u32,
) -> SignatureStatus {
    unsafe {
        core::mem::transmute::<
            *const (),
            fn(
                McuxClSessionHandle,
                McuxClKeyHandle,
                McuxClSignatureMode,
                *const u8,
                u32,
                *const u8,
                u32,
            ) -> SignatureStatus,
        >(0x1303b031 as _)(
            session,
            key,
            mode,
            input,
            input_size,
            signature,
            signature_size,
        )
    }
}
