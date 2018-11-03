#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface Sphere:UIView

-(void)getSphereVertexData:(float [])spherePositionCoords :(float [])sphereNormalCoords :(float [])sphereTexCoords :(unsigned short[])sphereElements;

-(void)processSphereData;

-(void)addTrianglefloat:(float[][3])single_vertex :(float[][3])single_normal :(float[][3])single_texture;

-(void)normalizeVector:(float [])v;

-(bool)isFoundIdentical:(const float)val1 :(const float)val2 :(const float)diff;


-(int)getNumberOfSphereVertices;


-(int)getNumberOfSphereElements;

@end
