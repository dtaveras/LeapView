#include "vtkLeapListener.h"
#include <QDebug>

vtkLeapListener::vtkLeapListener(){
}

//This is the one that we are using now change to the next constructor in the
//future
vtkLeapListener::vtkLeapListener(pqLeapView* view){
  this->view = view; 
  qRegisterMetaType<Frame>("Frame");
  QObject::connect(this, SIGNAL(frameChanged(Frame)),
		   view, SLOT(newFrame(Frame)));
}

vtkLeapListener::vtkLeapListener(vtkLeapController* leap){
  this->leapController = leap;
  qRegisterMetaType<Frame>("Frame");
  QObject::connect(this, SIGNAL(frameChanged(Frame)),
		   leap, SLOT(newFrame(Frame)));
}

void vtkLeapListener::onInit(const Controller& controller) {
  qDebug() << "Initialized";
}

void vtkLeapListener::onConnect(const Controller& controller) {
  qDebug() << "Connected";
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void vtkLeapListener::onDisconnect(const Controller& controller) {
  //Note: not dispatched when running in a debugger.
  qDebug() << "Disconnected";
}

void vtkLeapListener::onExit(const Controller& controller) {
  qDebug() << "Exited";
}

void vtkLeapListener::onFrame(const Controller& controller) {
  //qDebug() << " ON FRAME";
    // Get the most recent frame and report some basic information
    const Frame frame = controller.frame();
    /*qDebug() << "Frame id: " << frame.id()
              << ", timestamp: " << frame.timestamp()
              << ", hands: " << frame.hands().count()
              << ", fingers: " << frame.fingers().count()
              << ", tools: " << frame.tools().count()
	      << ", gestures: " << frame.gestures().count();
	      qDebug() << "Framerate: " << frame.currentFramesPerSecond();*/
    qDebug() << "[Gestures:" << frame.gestures().count() << "]";
    //if(frame.gestures().count() > 0){
    emit frameChanged(frame);

}

void vtkLeapListener::onFocusGained(const Controller& controller) {
  qDebug() << "Focus Gained";
}

void vtkLeapListener::onFocusLost(const Controller& controller) {
  qDebug() << "Focus Lost";
}
