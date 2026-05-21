#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to multi_floor_nav__msg__MissionRequest
/// MissionRequest.msg

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MissionRequest {
    /// 'robot1' / 'robot2' / 'auto'
    pub robot_id: std::string::String,


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
    pub mission_id: std::string::String,

}



impl Default for MissionRequest {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::MissionRequest::default())
  }
}

impl rosidl_runtime_rs::Message for MissionRequest {
  type RmwMsg = super::msg::rmw::MissionRequest;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        robot_id: msg.robot_id.as_str().into(),
        x: msg.x,
        y: msg.y,
        yaw: msg.yaw,
        target_floor: msg.target_floor,
        mission_id: msg.mission_id.as_str().into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        robot_id: msg.robot_id.as_str().into(),
      x: msg.x,
      y: msg.y,
      yaw: msg.yaw,
      target_floor: msg.target_floor,
        mission_id: msg.mission_id.as_str().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      robot_id: msg.robot_id.to_string(),
      x: msg.x,
      y: msg.y,
      yaw: msg.yaw,
      target_floor: msg.target_floor,
      mission_id: msg.mission_id.to_string(),
    }
  }
}


