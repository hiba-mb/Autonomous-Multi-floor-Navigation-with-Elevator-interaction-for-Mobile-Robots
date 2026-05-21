#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "multi_floor_nav__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__multi_floor_nav__msg__MissionRequest() -> *const std::ffi::c_void;
}

#[link(name = "multi_floor_nav__rosidl_generator_c")]
extern "C" {
    fn multi_floor_nav__msg__MissionRequest__init(msg: *mut MissionRequest) -> bool;
    fn multi_floor_nav__msg__MissionRequest__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<MissionRequest>, size: usize) -> bool;
    fn multi_floor_nav__msg__MissionRequest__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<MissionRequest>);
    fn multi_floor_nav__msg__MissionRequest__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<MissionRequest>, out_seq: *mut rosidl_runtime_rs::Sequence<MissionRequest>) -> bool;
}

// Corresponds to multi_floor_nav__msg__MissionRequest
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// MissionRequest.msg

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MissionRequest {
    /// 'robot1' / 'robot2' / 'auto'
    pub robot_id: rosidl_runtime_rs::String,


    // This member is not documented.
    #[allow(missing_docs)]
    pub x: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub y: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub yaw: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub target_floor: i32,

    /// UUID unique
    pub mission_id: rosidl_runtime_rs::String,

}



impl Default for MissionRequest {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !multi_floor_nav__msg__MissionRequest__init(&mut msg as *mut _) {
        panic!("Call to multi_floor_nav__msg__MissionRequest__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for MissionRequest {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { multi_floor_nav__msg__MissionRequest__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { multi_floor_nav__msg__MissionRequest__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { multi_floor_nav__msg__MissionRequest__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for MissionRequest {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for MissionRequest where Self: Sized {
  const TYPE_NAME: &'static str = "multi_floor_nav/msg/MissionRequest";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__multi_floor_nav__msg__MissionRequest() }
  }
}


