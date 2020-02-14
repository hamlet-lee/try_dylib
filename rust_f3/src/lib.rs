// https://rendered-obsolete.github.io/2016/03/21/rustlang-dynamic-library.html
// https://stackoverflow.com/questions/24145823/how-do-i-convert-a-c-string-into-a-rust-string-and-back-via-ffi
// https://www.reddit.com/r/rust/comments/9ibr9s/passing_a_string_to_c_efficiently/

extern crate libc;

use libc::c_char;
use std::ffi::CStr;
use std::str;
use std::ffi::CString;
use std::collections::HashMap;
use std::sync::Mutex;

// https://stackoverflow.com/questions/26388861/how-to-include-module-from-another-file-from-the-same-project
mod real;

#[macro_use]
extern crate lazy_static;

// https://stackoverflow.com/questions/34832583/global-mutable-hashmap-in-a-library
lazy_static! {
    static ref HASHMAP: Mutex<HashMap<u8, CString>> = {
        Mutex::new(HashMap::new())
    };
}

#[no_mangle]
pub extern "C" fn process (arg: *const c_char, constant_arg: *const c_char) -> *const c_char {
  // how?
  // https://doc.rust-lang.org/std/ffi/struct.CStr.html
  let arg = unsafe { CStr::from_ptr(arg) };
  let constant_arg = unsafe { CStr::from_ptr(constant_arg) };
  // https://doc.rust-lang.org/std/ffi/struct.CStr.html#method.to_string_lossy
  let ret = real::process(arg.to_str().unwrap(), constant_arg.to_str().unwrap());
  // let r = CString::new("abc").unwrap();
  let r = CString::new(ret).unwrap();
  let ptr = r.as_ptr();
  // https://stackoverflow.com/questions/34832583/global-mutable-hashmap-in-a-library
  HASHMAP.lock().unwrap().insert(ptr as u8, r);
  ptr
}

#[no_mangle]
pub extern "C" fn deleter (str: *const c_char) -> () {
  // how?
  // drop(str)
  // https://stackoverflow.com/questions/34832583/global-mutable-hashmap-in-a-library
  HASHMAP.lock().unwrap().remove(&(str as u8));
}