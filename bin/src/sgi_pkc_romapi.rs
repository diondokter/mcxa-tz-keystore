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

#[repr(transparent)]
pub struct McuxClKeyDescriptor([u32; 13]);
pub type McuxClKeyHandle = *const McuxClKeyDescriptor;

#[repr(transparent)]
pub struct SessionStatus(u32);
#[repr(transparent)]
pub struct KeyStatus(u32);

// *** SessionInit ***
type McuxClSessionInit = fn(
    session: McuxClSessionHandle,
    cpu_workarea_buffer: *const u32,
    cpu_workarea_length: u32,
    pkc_workarea_buffer: *const u32,
    pkc_workarea_length: u32,
) -> SessionStatus;
const MCUX_CL_SESSION_INIT_ADDR: *const u32 = 0x1301ff69 as _;
pub fn mcux_cl_session_init(
    session: McuxClSessionHandle,
    cpu_workarea_buffer: *const u32,
    cpu_workarea_length: u32,
    pkc_workarea_buffer: *const u32,
    pkc_workarea_length: u32,
) -> SessionStatus {
    unsafe {
        core::mem::transmute::<_, McuxClSessionInit>(MCUX_CL_SESSION_INIT_ADDR)(
            session,
            cpu_workarea_buffer,
            cpu_workarea_length,
            pkc_workarea_buffer,
            pkc_workarea_length,
        )
    }
}

// *** SessionCleanup ***
type McuxClSessionCleanup = fn(session: McuxClSessionHandle) -> SessionStatus;
const MCUX_CL_SESSION_CLEANUP_ADDR: u32 = 0x13020031;
pub fn mcux_cl_session_cleanup(session: McuxClSessionHandle) -> SessionStatus {
    unsafe {
        core::mem::transmute::<_, McuxClSessionCleanup>(MCUX_CL_SESSION_CLEANUP_ADDR)(session)
    }
}

// *** KeyInit ***
type McuxClKeyInit = fn(
    session: McuxClSessionHandle,
    key: McuxClKeyHandle,
    key_type: McuxClKeyType,
    key_data: *const u8,
    key_data_length: u32,
) -> KeyStatus;
const MCUX_CL_KEY_INIT_ADDR: u32 = 0x130217c5;
pub fn mcux_cl_key_init(
    session: McuxClSessionHandle,
    key: McuxClKeyHandle,
    key_type: McuxClKeyType,
    key_data: *const u8,
    key_data_length: u32,
) -> KeyStatus {
    unsafe {
        core::mem::transmute::<_, McuxClKeyInit>(MCUX_CL_KEY_INIT_ADDR)(
            session,
            key,
            key_type,
            key_data,
            key_data_length,
        )
    }
}

// *** KeyGenerateKeypair ***
type McuxClKeyGenerateKeypair = fn(
    session: McuxClSessionHandle,
    generation: McuxKeyGeneration,
    priv_key: McuxClKeyHandle,
    pub_key: McuxClKeyHandle,
) -> KeyStatus;
const MCUX_CL_KEY_GENERATE_KEYPAIR_ADDR: u32 = 0x130217c5;
pub fn mcux_cl_key_generate_keypair(
    session: McuxClSessionHandle,
    generation: McuxKeyGeneration,
    priv_key: McuxClKeyHandle,
    pub_key: McuxClKeyHandle,
) -> KeyStatus {
    unsafe {
        core::mem::transmute::<_, McuxClKeyGenerateKeypair>(MCUX_CL_KEY_GENERATE_KEYPAIR_ADDR)(
            session, generation, priv_key, pub_key,
        )
    }
}
