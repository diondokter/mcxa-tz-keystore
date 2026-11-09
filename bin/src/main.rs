#![no_std]
#![no_main]
#![feature(cmse_nonsecure_entry)]

use core::{panic::PanicInfo};

mod sgi_pkc_romapi;

#[panic_handler]
fn panic(_: &PanicInfo) -> ! {
    loop {}
}

#[unsafe(no_mangle)]
extern "cmse-nonsecure-entry" fn do_stuff_secure(num: u32) -> u32 {
    num * 2
}
