use std::ptr;
use std::ffi::{CString};

use std::os::raw::c_char;

extern crate libc;

use libc::{c_int};

#[allow(missing_copy_implementations)]
pub enum GLFWmonitor {}

#[allow(missing_copy_implementations)]
pub enum GLFWwindow {}

#[link(name = "glfw3")]
extern {
	fn glfwInit() -> c_int;
	fn glfwPollEvents() -> c_int;
	fn glfwCreateWindow(width: c_int, height: c_int, title: *const c_char, monitor: *mut GLFWmonitor, share: *mut GLFWwindow) -> *mut GLFWwindow;
	fn glfwWindowShouldClose(window: *mut GLFWwindow) -> c_int;
}

fn main() {
	unsafe {
		let string = CString::new("Something".as_bytes()).unwrap(); //tricky stuff. If written in one line string would vanish!
		let title = string.as_bytes_with_nul().as_ptr() as *const c_char;

		glfwInit();

		let window = glfwCreateWindow(800 as c_int, 600 as c_int, title, ptr::null_mut(), ptr::null_mut());


		println!("WORKING");

		loop {
			glfwPollEvents();

			if glfwWindowShouldClose(window) != 0 {
				break;
			}
		};

		println!("ENDED");
	};
}