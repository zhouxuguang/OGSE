//
//  GeoCentric.h
//  CoordTran
//
//  Created by Zhou,Xuguang on 2018/12/26.
//  Copyright © 2018年 zhouxuguang. All rights reserved.
//

#ifndef GeoCentric_hpp
#define GeoCentric_hpp

#include <stdio.h>

/***************************************************************************/
/*
 *                              DEFINES
 */
#define GEOCENT_NO_ERROR        0x0000
#define GEOCENT_LAT_ERROR       0x0001
#define GEOCENT_LON_ERROR       0x0002
#define GEOCENT_A_ERROR         0x0004
#define GEOCENT_B_ERROR         0x0008
#define GEOCENT_A_LESS_B_ERROR  0x0010


/***************************************************************************/
/*
 *                              FUNCTION PROTOTYPES
 */

/* ensure proper linkage to c++ programs */
#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct
    {
        double Geocent_a;        /* Semi-major axis of ellipsoid in meters */
        double Geocent_b;        /* Semi-minor axis of ellipsoid           */
        double Geocent_a2;       /* Square of semi-major axis */
        double Geocent_b2;       /* Square of semi-minor axis */
        double Geocent_e2;       /* Eccentricity squared  */
        double Geocent_ep2;      /* 2nd eccentricity squared */
    } GeocentricInfo;
    
    void Init_Geocentric( GeocentricInfo *gi );
    long Set_Geocentric_Parameters( GeocentricInfo *gi,
                                      double a,
                                      double b);
    
    /*
     * The function Set_Geocentric_Parameters receives the ellipsoid parameters
     * as inputs and sets the corresponding state variables.
     *
     *    a  : Semi-major axis, in meters.          (input)
     *    b  : Semi-minor axis, in meters.          (input)
     */
    
    
    void Get_Geocentric_Parameters ( GeocentricInfo *gi,
                                       double *a,
                                       double *b);
    
    /*
     * The function Get_Geocentric_Parameters returns the ellipsoid parameters
     * to be used in geocentric coordinate conversions.
     *
     *    a  : Semi-major axis, in meters.          (output)
     *    b  : Semi-minor axis, in meters.          (output)
     */
    
    
    long Convert_Geodetic_To_Geocentric ( GeocentricInfo *gi,
                                            double Latitude,
                                            double Longitude,
                                            double Height,
                                            double *X,
                                            double *Y,
                                            double *Z);
    /*
     * The function Convert_Geodetic_To_Geocentric converts geodetic coordinates
     * (latitude, longitude, and height) to geocentric coordinates (X, Y, Z),
     * according to the current ellipsoid parameters.
     *
     *    Latitude  : Geodetic latitude in radians                     (input)
     *    Longitude : Geodetic longitude in radians                    (input)
     *    Height    : Geodetic height, in meters                       (input)
     *    X         : Calculated Geocentric X coordinate, in meters.   (output)
     *    Y         : Calculated Geocentric Y coordinate, in meters.   (output)
     *    Z         : Calculated Geocentric Z coordinate, in meters.   (output)
     *
     */
    
    
    void Convert_Geocentric_To_Geodetic (GeocentricInfo *gi,
                                            double X,
                                            double Y,
                                            double Z,
                                            double *Latitude,
                                            double *Longitude,
                                            double *Height);
    /*
     * The function Convert_Geocentric_To_Geodetic converts geocentric
     * coordinates (X, Y, Z) to geodetic coordinates (latitude, longitude,
     * and height), according to the current ellipsoid parameters.
     *
     *    X         : Geocentric X coordinate, in meters.         (input)
     *    Y         : Geocentric Y coordinate, in meters.         (input)
     *    Z         : Geocentric Z coordinate, in meters.         (input)
     *    Latitude  : Calculated latitude value in radians.       (output)
     *    Longitude : Calculated longitude value in radians.      (output)
     *    Height    : Calculated height value, in meters.         (output)
     */
    
    
#ifdef __cplusplus
}
#endif

#endif /* GeoCentric_hpp */
