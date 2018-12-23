//
//  GeoMath.hpp
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#ifndef EARTH_ENGINE_CORE_GEOMATH_H
#define EARTH_ENGINE_CORE_GEOMATH_H

#include "EarthEngineDefine.h"
#include "Vector3.h"

USING_NS_MATH3D

const double WGS_84_RADIUS_EQUATOR = 6378140;

EARTH_CORE_NAMESPACE_BEGIN

    /**
     * Useful calculations for lat/long points.
     * Converted from http://www.movable-type.co.uk/scripts/latlong.html
     */
    class EARTH_CORE_EXPORT GeoMath
    {
    public:
        /**
         *Computes the distance between the given points in meters using the Haversine formula
         * @param lat1Rad
         *    The start latitude in radians
         * @param lon1Rad
         *    The start longitude in radians
         * @param lat2Rad
         *    The end latitude in radians
         * @param lon2Rad
         *    The end longitude in radians
         * @param radius
         *    The radius of the earth in meters
         * @returns
         *    The distance between the two points in meters
         */
        static double distance(double lat1Rad, double lon1Rad,
                               double lat2Rad, double lon2Rad,
                               double radius = WGS_84_RADIUS_EQUATOR);
        
        /**
         *Computes the distance covered by the given path in meters using the Haversine formula
         *Assumes the points in Lon, Lat in degrees
         */
        static double distance(const std::vector< Vector3 > &points, double radius = WGS_84_RADIUS_EQUATOR);
        
        /**
         * Computes the distance between two points, in meters.
         */
        //static double distance(const osg::Vec3d& p1, const osg::Vec3d& p2, const SpatialReference* srs);
        
        
        /**
         * Computes the initial bearing from one point to the next in radians
         * @param lat1Rad
         *    The start latitude in radians
         * @param lon1Rad
         *    The start longitude in radians
         * @param lat2Rad
         *    The end latitude in radians
         * @param lon2Rad
         *    The end longitude in radians
         * @returns
         *    The initial bearing in radians
         */
        static double bearing(double lat1Rad, double lon1Rad,
                              double lat2Rad, double lon2Rad);
        
        /**
         *Computes the midpoint between two points
         * @param lat1Rad
         *    The start latitude in radians
         * @param lon1Rad
         *    The start longitude in radians
         * @param lat2Rad
         *    The end latitude in radians
         * @param lon2Rad
         *    The end longitude in radians
         * @param out_latRad
         *    The latitude of the midpoint in radians
         * @param out_lonRad
         *    The longitude of the midpoint in radians
         */
        static void midpoint(double lat1Rad, double lon1Rad,
                             double lat2Rad, double lon2Rad,
                             double &out_latRad, double &out_lonRad);
        
        /**
         * Computes the destination point given a start point, a bearing and a distance
         * @param lat1Rad
         *    The latitude in radians
         * @param lon1Rad
         *    The longitude in radians
         * @param bearingRad
         *    The bearing in radians
         * @param distance
         *    The distance in meters
         * @param out_latRad
         *    The destination point's latitude in radians
         * @param out_lonRad
         *    The destination points' longitude in radians
         * @param radius
         *    The radius of the earth in meters
         */
        static void destination(double lat1Rad, double lon1Rad,
                                double bearingRad, double distance,
                                double &out_latRad, double &out_lonRad,
                                double radius = WGS_84_RADIUS_EQUATOR);
        
        /**
         * Calculates the minimum and maximum latitudes along a great circle
         * between the two geodetic input points.
         */
        static void greatCircleMinMaxLatitude(
                                              double lat1Rad, double lon1Rad,
                                              double lat2Rad, double lon2Rad,
                                              double& out_minLatRad, double& out_maxLatRad);
        
        
        /**
         * Computes the distance between two points in meters following a rhumb line
         * @param lat1Rad
         *   The start latitude in radians
         * @param lon1Rad
         *   The start longitude in radians
         * @param lat2Rad
         *   The end latitude in radians
         * @param lon2Rad
         *   The end longitude in radians
         * @param radius
         *    The radius of the earth in meters
         * @returns
         *   The distance between the two points in meters following a rhumb line
         */
        static double rhumbDistance(double lat1Rad, double lon1Rad,
                                    double lat2Rad, double lon2Rad,
                                    double radius = WGS_84_RADIUS_EQUATOR);
        
        
        /**
         * Computes the distance between two points in meters following a rhumb line
         * Assumes the points are in Lon, Lat in degrees
         */
        static double rhumbDistance(const std::vector< Vector3 > &points, double radius = WGS_84_RADIUS_EQUATOR);
        
        
        /**
         *Computes the bearing of the rhumb line between two points in radians
         * @param lat1Rad
         *    The start latitude in radians
         * @param lon1Rad
         *    The start longitude in radians
         * @param lat2Rad
         *    The end latitude in radians
         * @param lon2Rad
         *    The end longitude in radians
         */
        static double rhumbBearing(double lat1Rad, double lon1Rad,
                                   double lat2Rad, double lon2Rad);
        
        /** Computes the destination point given a start point, a bearing and a distance along a rhumb line
         * @param lat1Rad
         *    The latitude in radians
         * @param lon1Rad
         *    The longitude in radians
         * @param bearingRad
         *    The bearing in radians
         * @param distance
         *    The distance in meters
         * @param out_latRad
         *    The destination point's latitude in radians
         * @param out_lonRad
         *    The destination points' longitude in radians
         * @param radius
         *    The radius of the earth in meters
         */
        static void rhumbDestination(double lat1Rad, double lon1Rad,
                                     double bearing, double distance,
                                     double &out_latRad, double &out_lonRad,
                                     double radius = WGS_84_RADIUS_EQUATOR);
        
        
        /**
         * Computes the intersection(s) of a line with a sphere.
         * Returns the number of intersections: 0, 1, or 2.
         */
        static unsigned interesectLineWithSphere(const Vector3& p0,
                                                 const Vector3& p1,
                                                 double            radius,
                                                 Vector3&       out_i0,
                                                 Vector3&       out_i1);
        
        /**
         * Whether the target point is visible from the eye point in a
         * spherical earth approximation with the given Radius.
         */
        static bool isPointVisible(const Vector3& eye,
                                   const Vector3& target,
                                   double radius = WGS_84_RADIUS_EQUATOR);
    };

EARTH_CORE_NAMESPACE_END

#endif
