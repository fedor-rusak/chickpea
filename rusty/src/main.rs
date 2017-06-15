use std::ptr;
use std::ffi::{CString, CStr};

use std::os::raw::c_char;

extern crate libc;

use libc::{c_int, c_float, c_void, c_uint};

#[allow(missing_copy_implementations)]
pub enum GLFWmonitor {}

#[allow(missing_copy_implementations)]
pub enum GLFWwindow {}

#[allow(missing_copy_implementations)]
pub enum GLenum {}

pub static GL_COLOR_BUFFER_BIT: c_uint = 0x00004000; //it is a macro constant :(

#[link(name = "glfw3")]
extern {
	fn glfwInit() -> c_int;
	fn glfwPollEvents() -> c_int;
	fn glfwCreateWindow(width: c_int, height: c_int, title: *const c_char, monitor: *mut GLFWmonitor, share: *mut GLFWwindow) -> *mut GLFWwindow;
	fn glfwMakeContextCurrent(window: *mut GLFWwindow) -> c_void;
	fn glfwWindowShouldClose(window: *mut GLFWwindow) -> c_int;
	fn glfwSwapBuffers(window: *mut GLFWwindow) -> c_void;
	fn glfwSetWindowSizeCallback(window: *mut GLFWwindow, onResizeCallback: extern fn(window: *mut GLFWwindow, i32, i32)) -> c_void;
}

#[link(name = "glew32")]
extern "stdcall" { //this is some wicked mumbo-jumbo for windows macro and dll
	fn glewInit() -> c_int;
}

#[link(name = "OpenGL32")]
extern "stdcall" {
	fn glClearColor(r: c_float, g: c_float, b: c_float, a: c_float) -> c_void;
	fn glClear(bitmask: c_uint) -> c_void;
}

#[allow(missing_copy_implementations)]
#[allow(non_camel_case_types)]
pub enum duk_context {}

#[link(name = "duktape")]
extern "stdcall" {
	fn duk_create_heap(
		alloc_func: *mut c_void,
		realloc_func: *mut c_void,
		free_func: *mut c_void,
		heap_udata: *mut c_void,
		fatal_handler: *mut c_void) -> *mut duk_context;
	fn duk_eval_raw(context: *mut duk_context, script: *const c_char, wtf1: c_int, wtf2: c_int) -> c_void;
	fn duk_push_c_function(context: *mut duk_context, callback: extern fn(context: *mut duk_context) -> c_int, arg_count: c_int) -> c_int;
	fn duk_put_global_string(context: *mut duk_context, script: *const c_char);
	fn duk_to_string(context: *mut duk_context, index: c_int) -> *const c_char;
}

#[allow(unused_variables)]
extern fn on_resize_callback(window: *mut GLFWwindow, width: i32, height: i32) {
    println!("I'm called from C with value {0} and {1}", width, height);
}

extern fn duk_print(context: *mut duk_context) -> c_int {
	unsafe {
		let to_print = CStr::from_ptr(duk_to_string(context, 0));
		println!("I'm called from duktape! {0}", to_print.to_str().unwrap());
	}
	return 0;
}

fn main() {
	unsafe {
		let string = CString::new("Something".as_bytes()).unwrap(); //tricky stuff. If written in one line string would vanish!
		let title = string.as_bytes_with_nul().as_ptr() as *const c_char;

		glfwInit();

		let window = glfwCreateWindow(800 as c_int, 600 as c_int, title, ptr::null_mut(), ptr::null_mut());

		glfwSetWindowSizeCallback(window, on_resize_callback);

		glfwMakeContextCurrent(window);

		println!("WORKING GLFW");

		if glewInit() == 0 {
			println!("WORKING GLEW");
		}
		else {
			println!("FAILED GLEW");
		}

		glClearColor(0.3, 0.4, 0.1, 1.0);

		loop {
			glfwPollEvents();

			if glfwWindowShouldClose(window) == 1 {
				break;
			}

			glClear(GL_COLOR_BUFFER_BIT);

			glfwSwapBuffers(window);
		};

		println!("ENDED");

		println!("JS?");
		let js_context = duk_create_heap(ptr::null_mut(),ptr::null_mut(),ptr::null_mut(),ptr::null_mut(),ptr::null_mut());


		let function_name_string = CString::new("print".as_bytes()).unwrap(); //tricky stuff. If written in one line string would vanish!
		let function_name = function_name_string.as_bytes_with_nul().as_ptr() as *const c_char;

		duk_push_c_function(js_context, duk_print, 1);
		duk_put_global_string(js_context, function_name);

		let script_string = CString::new("print(42);".as_bytes()).unwrap(); //tricky stuff. If written in one line string would vanish!
		let script = script_string.as_bytes_with_nul().as_ptr() as *const c_char;
		duk_eval_raw(js_context, script, 0, 0 | (1 << 3) | (1 << 7) | (1 << 9) | (1 << 10) | (1 << 11));
		println!("YES!");
	};
}