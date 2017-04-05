/*
 * AerialMapDisplay.h
 *
 *  Copyright (c) 2014 Gaeth Cross. Apache 2 License.
 *
 *  This file is part of rviz_satellite.
 *
 *	Created on: 07/09/2014
 */

#ifndef AERIAL_MAP_DISPLAY_H
#define AERIAL_MAP_DISPLAY_H

// NOTE: workaround for issue: https://bugreports.qt.io/browse/QTBUG-22829
#ifndef Q_MOC_RUN
#include <ros/ros.h>
#include <ros/time.h>
#include <rviz/display.h>
#include <sensor_msgs/NavSatFix.h>

#include <OGRE/OgreTexture.h>
#include <OGRE/OgreMaterial.h>
#endif  //  Q_MOC_RUN

#include <QObject>
#include <QtConcurrentRun>
#include <QFuture>
#include <QByteArray>
#include <QFile>
#include <QNetworkRequest>

#include <memory>
#include <tileloader.h>

#include <rviz_satellite/ReadTakeoffGps.h>

#include <QTimer>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <std_msgs/Float64.h>

using namespace cv;

namespace Ogre {
    class ManualObject;
}

namespace rviz {

    class FloatProperty;
    class IntProperty;
    class Property;
    class RosTopicProperty;
    class StringProperty;
    class TfFrameProperty;
    class EnumProperty;

    /**
     * @class AerialMapDisplay
     * @brief Displays a satellite map along the XY plane.
     */
    class AerialMapDisplay : public Display {
        Q_OBJECT
    public:
        AerialMapDisplay();
        virtual ~AerialMapDisplay();

        // Overrides from Display
        virtual void onInitialize();
        virtual void fixedFrameChanged();
        virtual void reset();
        virtual void update(float, float);

        protected
Q_SLOTS:
        void updateDynamicReload();
        void updateAlpha();
        void updateTopic();
        void updateFrame();
        void updateDrawUnder();
        void updateObjectURI();
        void updateZoom();
        void updateBlocks();
        void updateFrameConvention();
        void refresh();

        //  slots for TileLoader messages
        void initiatedRequest(QNetworkRequest request);
        void receivedImage(QNetworkRequest request);
        void finishedLoading();
        void errorOcurred(QString description);

    protected:
        // overrides from Display
        virtual void onEnable();
        virtual void onDisable();

        virtual void subscribe();
        virtual void unsubscribe();

        void navFixCallback(const sensor_msgs::NavSatFixConstPtr &msg);

        void loadImagery();

        void assembleScene();

        void clear();

        void clearGeometry();

        void transformAerialMap();

        unsigned int map_id_;
        unsigned int scene_id_;

        /// Instance of a tile w/ associated ogre data

        struct MapObject {
            Ogre::ManualObject *object;
            Ogre::TexturePtr texture;
            Ogre::MaterialPtr material;
        };
        std::vector<MapObject> objects_;

        ros::Subscriber coord_sub_;
        ros::ServiceServer service;

        //  properties
        RosTopicProperty *topic_property_;
        TfFrameProperty *frame_property_;
        Property *dynamic_reload_property_;
        StringProperty *object_uri_property_;
        IntProperty *zoom_property_;
        IntProperty *blocks_property_;
        FloatProperty *resolution_property_;
        FloatProperty *alpha_property_;
        Property *draw_under_property_;
        Property *draw_mapscache_;
        EnumProperty * frame_convention_property_;
        StringProperty *lat_property_;
        StringProperty *lon_property_;

        float alpha_;
        bool draw_under_;
        std::string object_uri_;
        int zoom_;
        int blocks_;

        //  tile management
        bool dirty_;
        bool received_msg_;
        sensor_msgs::NavSatFix current_ref_fix_;
        std::shared_ptr<TileLoader> loader_;

        QTimer *timer;
        image_transport::ImageTransport it_;
        image_transport::Subscriber image_sub_;
        ros::Subscriber compass_sub;
    };

} // namespace rviz

#endif
