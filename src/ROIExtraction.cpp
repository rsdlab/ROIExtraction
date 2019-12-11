// -*- C++ -*-
/*!
 * @file  ROIExtraction.cpp
 * @brief ROI extraction from ArUco maker corners information
 * @date $Date$
 *
 * $Id$
 */

#include "ROIExtraction.h"

// Module specification
// <rtc-template block="module_spec">
static const char* roiextraction_spec[] =
  {
    "implementation_id", "ROIExtraction",
    "type_name",         "ROIExtraction",
    "description",       "ROI extraction from ArUco maker corners information",
    "version",           "1.0.0",
    "vendor",            "Masayuki Fukao, Robot System Design Laboratory, Meijo University",
    "category",          "Image Processing",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.front_left_marker_id", "1",
    "conf.default.front_right_marker_id", "2",
    "conf.default.back_left_marker_id", "3",
    "conf.default.back_right_marker_id", "4",

    // Widget
    "conf.__widget__.front_left_marker_id", "text",
    "conf.__widget__.front_right_marker_id", "text",
    "conf.__widget__.back_left_marker_id", "text",
    "conf.__widget__.back_right_marker_id", "text",
    // Constraints

    "conf.__type__.front_left_marker_id", "int",
    "conf.__type__.front_right_marker_id", "int",
    "conf.__type__.back_left_marker_id", "int",
    "conf.__type__.back_right_marker_id", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
ROIExtraction::ROIExtraction(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_CornerPointsIn("CornerPoints", m_CornerPoints),
    m_CornersOut("Corners", m_Corners)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ROIExtraction::~ROIExtraction()
{
}



RTC::ReturnCode_t ROIExtraction::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("CornerPoints", m_CornerPointsIn);
  
  // Set OutPort buffer
  addOutPort("Corners", m_CornersOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("front_left_marker_id", m_front_left_marker_id, "1");
  bindParameter("front_right_marker_id", m_front_right_marker_id, "2");
  bindParameter("back_left_marker_id", m_back_left_marker_id, "3");
  bindParameter("back_right_marker_id", m_back_right_marker_id, "4");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

RTC::ReturnCode_t ROIExtraction::onActivated(RTC::UniqueId ec_id)
{
  m_CornerPoints.markerCorners.length();
  m_CornerPoints.ids.length();

  //サイズの確保
  m_Corners.cornerdata.length(4);
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ROIExtraction::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t ROIExtraction::onExecute(RTC::UniqueId ec_id)
{
  //入力データが新しいか確認
  if(m_CornerPointsIn.isNew())
    {
      //データの取り込み
      m_CornerPointsIn.read();

      //マーカーIDを並び替え、マーカーの中心の座標を限定エリア範囲して設定する
      //左上が０、右上が１，右下が２，左下が３とアルコマーカーの順番と同じ
      for(int i = 0 ; i < m_CornerPoints.ids.length() ; i ++ ){
	if(m_CornerPoints.ids[i] == m_back_left_marker_id){
	  //corner0
	  m_Corners.cornerdata[0].x = (m_CornerPoints.markerCorners[i].point1.x + m_CornerPoints.markerCorners[i].point2.x) /2;
	  m_Corners.cornerdata[0].y = (m_CornerPoints.markerCorners[i].point1.y + m_CornerPoints.markerCorners[i].point4.y) /2;
	  
	}else if(m_CornerPoints.ids[i] == m_back_right_marker_id){
	  //corner1
	  m_Corners.cornerdata[1].x = (m_CornerPoints.markerCorners[i].point1.x + m_CornerPoints.markerCorners[i].point2.x) /2;
	  m_Corners.cornerdata[1].y = (m_CornerPoints.markerCorners[i].point1.y + m_CornerPoints.markerCorners[i].point4.y) /2;
	  
	}else if(m_CornerPoints.ids[i] == m_front_right_marker_id){
	  //corner2
	  m_Corners.cornerdata[2].x = (m_CornerPoints.markerCorners[i].point1.x + m_CornerPoints.markerCorners[i].point2.x) /2;
	  m_Corners.cornerdata[2].y = (m_CornerPoints.markerCorners[i].point1.y + m_CornerPoints.markerCorners[i].point4.y) /2;

	}else if(m_CornerPoints.ids[i] == m_front_left_marker_id){
	  //corner3
	  m_Corners.cornerdata[3].x = (m_CornerPoints.markerCorners[i].point1.x + m_CornerPoints.markerCorners[i].point2.x) /2;
	  m_Corners.cornerdata[3].y = (m_CornerPoints.markerCorners[i].point1.y + m_CornerPoints.markerCorners[i].point4.y) /2;

	}else{
	  //no corner
	  printf("Maker Corner Not Found\n");
	}
      }     
    }

  m_CornersOut.write();
  return RTC::RTC_OK;
}

extern "C"
{
 
  void ROIExtractionInit(RTC::Manager* manager)
  {
    coil::Properties profile(roiextraction_spec);
    manager->registerFactory(profile,
                             RTC::Create<ROIExtraction>,
                             RTC::Delete<ROIExtraction>);
  }
  
};


