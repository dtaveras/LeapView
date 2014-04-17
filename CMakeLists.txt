#--------------------------------------------------
# Find and Use ParaView
#--------------------------------------------------
cmake_minimum_required(VERSION 2.8)
IF (ParaView_SOURCE_DIR)
  INCLUDE_DIRECTORIES(
    ${PARAVIEW_INCLUDE_DIRS}
    ${VTK_INCLUDE_DIRS}
  )
ELSE (ParaView_SOURCE_DIR)
  FIND_PACKAGE(ParaView REQUIRED)
  INCLUDE(${PARAVIEW_USE_FILE})
ENDIF (ParaView_SOURCE_DIR)

IF(PARAVIEW_BUILD_QT_GUI)
#  INCLUDE(${QT_USE_FILE})
# INCLUDE_DIRECTORIES(${PARAVIEW_GUI_INCLUDE_DIRS})

  QT4_WRAP_CPP(MOC_SRCS
    pqLeapView.h
    pqActiveLeapViewOptions.h
  )

  QT4_WRAP_UI(UI_SRCS
#    pqQuadViewOptions.ui
  )

  add_paraview_view_module(
    VIEW1_IFACE VIEW1_IFACE_SRCS
    VIEW_TYPE pqLeapView
    VIEW_NAME "Leap View"
    VIEW_XML_GROUP "views"
    VIEW_XML_NAME "LeapView")

  ADD_PARAVIEW_VIEW_OPTIONS(
    VIEW1_OPTIONS_IFACE VIEW1_OPTIONS_IFACE_SRCS
    VIEW_TYPE pqLeapView
    ACTIVE_VIEW_OPTIONS pqActiveLeapViewOptions
  )

  ADD_PARAVIEW_PLUGIN(LeapView "1.0"

    SERVER_MANAGER_SOURCES
      vtkPVLeapRenderView.cxx
#      vtkPVQuadViewInformation.cxx
#      vtkQuadRepresentation.cxx
#      vtkSMQuadViewProxy.cxx

    SERVER_MANAGER_XML
      LeapMotionViews.xml

    GUI_INTERFACES
      ${VIEW1_IFACE}
      ${VIEW1_OPTIONS_IFACE}

    GUI_SOURCES
      ${MOC_SRCS}
      ${UI_SRCS}
      ${VIEW1_IFACE_SRCS}
      ${VIEW1_OPTIONS_IFACE_SRCS}
      pqActiveLeapViewOptions.cxx

      pqLeapView.cxx
      pqActiveLeapViewOptions.cxx
  )

ENDIF(PARAVIEW_BUILD_QT_GUI)

# Add testing if necessary
if (BUILD_TESTING)
  add_subdirectory(Testing)
endif (BUILD_TESTING)
