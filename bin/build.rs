use std::env;
use std::fs::File;
use std::io::Write;
use std::path::PathBuf;

fn main() {
    let out = &PathBuf::from(env::var_os("OUT_DIR").unwrap());

    // Flash bounds are shared; RAM bounds are selected per binary below.
    File::create(out.join("link.x"))
        .unwrap()
        .write_all(include_bytes!("link.x"))
        .unwrap();

    println!("cargo:rustc-link-search={}", out.display());
    println!("cargo:rustc-link-arg-bins=-Tlink.x");
    println!("cargo:rerun-if-changed=link.x");
    println!("cargo:rustc-link-arg-bins=--cmse-implib");
    println!("cargo:rustc-link-arg-bins=--out-implib=target/veneers.o");
}
