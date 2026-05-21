#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



#[link(name = "multi_floor_nav__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__multi_floor_nav__srv__IntTrigger_Request() -> *const std::ffi::c_void;
}

#[link(name = "multi_floor_nav__rosidl_generator_c")]
extern "C" {
    fn multi_floor_nav__srv__IntTrigger_Request__init(msg: *mut IntTrigger_Request) -> bool;
    fn multi_floor_nav__srv__IntTrigger_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<IntTrigger_Request>, size: usize) -> bool;
    fn multi_floor_nav__srv__IntTrigger_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<IntTrigger_Request>);
    fn multi_floor_nav__srv__IntTrigger_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<IntTrigger_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<IntTrigger_Request>) -> bool;
}

// Corresponds to multi_floor_nav__srv__IntTrigger_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct IntTrigger_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub req_int: i32,

}



impl Default for IntTrigger_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !multi_floor_nav__srv__IntTrigger_Request__init(&mut msg as *mut _) {
        panic!("Call to multi_floor_nav__srv__IntTrigger_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for IntTrigger_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { multi_floor_nav__srv__IntTrigger_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { multi_floor_nav__srv__IntTrigger_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { multi_floor_nav__srv__IntTrigger_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for IntTrigger_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for IntTrigger_Request where Self: Sized {
  const TYPE_NAME: &'static str = "multi_floor_nav/srv/IntTrigger_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__multi_floor_nav__srv__IntTrigger_Request() }
  }
}


#[link(name = "multi_floor_nav__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__multi_floor_nav__srv__IntTrigger_Response() -> *const std::ffi::c_void;
}

#[link(name = "multi_floor_nav__rosidl_generator_c")]
extern "C" {
    fn multi_floor_nav__srv__IntTrigger_Response__init(msg: *mut IntTrigger_Response) -> bool;
    fn multi_floor_nav__srv__IntTrigger_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<IntTrigger_Response>, size: usize) -> bool;
    fn multi_floor_nav__srv__IntTrigger_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<IntTrigger_Response>);
    fn multi_floor_nav__srv__IntTrigger_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<IntTrigger_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<IntTrigger_Response>) -> bool;
}

// Corresponds to multi_floor_nav__srv__IntTrigger_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct IntTrigger_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub message: rosidl_runtime_rs::String,

}



impl Default for IntTrigger_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !multi_floor_nav__srv__IntTrigger_Response__init(&mut msg as *mut _) {
        panic!("Call to multi_floor_nav__srv__IntTrigger_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for IntTrigger_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { multi_floor_nav__srv__IntTrigger_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { multi_floor_nav__srv__IntTrigger_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { multi_floor_nav__srv__IntTrigger_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for IntTrigger_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for IntTrigger_Response where Self: Sized {
  const TYPE_NAME: &'static str = "multi_floor_nav/srv/IntTrigger_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__multi_floor_nav__srv__IntTrigger_Response() }
  }
}






#[link(name = "multi_floor_nav__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__multi_floor_nav__srv__IntTrigger() -> *const std::ffi::c_void;
}

// Corresponds to multi_floor_nav__srv__IntTrigger
#[allow(missing_docs, non_camel_case_types)]
pub struct IntTrigger;

impl rosidl_runtime_rs::Service for IntTrigger {
    type Request = IntTrigger_Request;
    type Response = IntTrigger_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__multi_floor_nav__srv__IntTrigger() }
    }
}


