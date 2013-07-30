/** Copyright (C) 2013 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/
#include "Bridges/General/DX9Device.h"
#include "../General/GSd3d.h"
#include "Direct3D9Headers.h"
#include "../General/GStextures.h"
#include "../General/DXbinding.h"
#include "DX9model.h"
#include "DX9shapes.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"

#define __GETR(x) ((x & 0x0000FF))/255.0
#define __GETG(x) ((x & 0x00FF00)>>8)/255.0
#define __GETB(x) ((x & 0xFF0000)>>16)/255.0

bool d3dMode = false;
bool d3dHidden = false;
bool d3dZWriteEnable = true;
double projection_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}, transformation_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

namespace enigma_user
{

void d3d_start()
{

}

void d3d_end()
{

}

bool d3d_get_mode()
{
    return d3dMode;
}

void d3d_set_hidden(bool enable)
{
    d3dHidden = enable;
}   // TODO: Write function

void d3d_set_zwriteenable(bool enable)
{
	d3ddev->SetRenderState(D3DRS_ZENABLE, enable);    // enable/disable the z-buffer
}

void d3d_set_lighting(bool enable)
{
	d3ddev->SetRenderState(D3DRS_LIGHTING, enable);    // enable/disable the 3D lighting
}

void d3d_set_fog(bool enable, int color, double start, double end)
{
  d3d_set_fog_enabled(enable);
  d3d_set_fog_color(color);
  d3d_set_fog_start(start);
  d3d_set_fog_end(end);
  d3d_set_fog_hint(rs_nicest);
  d3d_set_fog_mode(rs_linear);
}

void d3d_set_fog_enabled(bool enable)
{
	d3ddev->SetRenderState(D3DRS_FOGENABLE, enable);
}

void d3d_set_fog_mode(int mode)
{

}

void d3d_set_fog_hint(int mode) {

}

void d3d_set_fog_color(int color)
{
	HRESULT hr;
	hr = d3ddev->SetRenderState(
                    D3DRS_FOGCOLOR,
                    0x00FFFFFF); // Highest 8 bits are not used.
	//if(FAILED(hr))
		//return hr;
}

void d3d_set_fog_start(double start)
{
    d3ddev->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&start));
}

void d3d_set_fog_end(double end)
{
    d3ddev->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&end));
}

void d3d_set_fog_density(double density)
{
	d3ddev->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&density));
}

void d3d_set_culling(bool enable)
{

}

void d3d_set_culling_mode(int mode) {

}

void d3d_set_culling_orientation(int mode) {

}

void d3d_set_render_mode(int face, int fill)
{

}
void d3d_set_line_width(float value) {

}

void d3d_set_point_size(float value) {

}

void d3d_depth_clear() {
  d3d_depth_clear_value(1.0f);
}

void d3d_depth_clear_value(float value) {
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), value, 0);
}

void d3d_depth_operator(int mode) {

}

void d3d_set_perspective(bool enable)
{

}

void d3d_set_depth(double dep)
{

}

void d3d_set_shading(bool smooth)
{

}

}

namespace enigma {
  extern unsigned char currentcolor[4];
}

namespace enigma_user
{

void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto,gs_scalar xup, gs_scalar yup, gs_scalar zup)
{

	D3DXVECTOR3 vEyePt( xfrom, yfrom, zfrom );
	D3DXVECTOR3 vLookatPt( xto, yto, zto );
	D3DXVECTOR3 vUpVec( xup, yup, zup );

	// Get D3DX to fill in the matrix values
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );

	// Set our view matrix
	d3ddev->SetTransform( D3DTS_VIEW, &matView ); 
	
}

void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto,gs_scalar xup, gs_scalar yup, gs_scalar zup,double angle,double aspect,double znear,double zfar)
{
	D3DXVECTOR3 vEyePt( xfrom, yfrom, zfrom );
	D3DXVECTOR3 vLookatPt( xto, yto, zto );
	D3DXVECTOR3 vUpVec( xup, yup, zup );

	// Get D3DX to fill in the matrix values
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );

	// Set our view matrix
	d3ddev->SetTransform( D3DTS_VIEW, &matView ); 
	
	D3DXMATRIX matProj;

	D3DXMatrixPerspectiveFovLH( &matProj, angle, aspect, znear, zfar );
	d3ddev->SetTransform( D3DTS_PROJECTION, &matProj );
}

void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, double angle)
{
	D3DVIEWPORT9 viewData = { x, y, width, height, 0.0f, 1.0f };
	HRESULT hr;

	hr = d3ddev->SetViewport(&viewData);
	//if(FAILED(hr))
		//return hr;
}

void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, double angle)
{
	D3DVIEWPORT9 viewData = { x, y, width, height, 0.0f, 1.0f };
	HRESULT hr;

	hr = d3ddev->SetViewport(&viewData);
	//if(FAILED(hr))
		//return hr;
		
	D3DXMATRIX matProjection;    // the projection transform matrix

	D3DXMatrixPerspectiveFovLH(&matProjection,
                           D3DXToRadian(45),    // the horizontal field of view
                           (FLOAT)width / (FLOAT)height,    // aspect ratio
                           1.0f,    // the near view-plane
                           100.0f);    // the far view-plane

	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection transform
}

void d3d_draw_wall(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep)
{

}

void d3d_draw_floor(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep)
{

}

void d3d_draw_block(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed)
{

}

void d3d_draw_cylinder(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{

}

void d3d_draw_cone(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{

}

void d3d_draw_ellipsoid(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps)
{

}

void d3d_draw_icosahedron(int texId) {
    texture_use(get_texture(texId));
}

void d3d_draw_torus(gs_scalar x1, gs_scalar y1, gs_scalar z1, int texId, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI) {

}

// ***** TRANSFORMATIONS BEGIN *****
void d3d_transform_set_identity()
{

}

void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{

}
void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{

}
void d3d_transform_add_rotation_x(double angle)
{

}
void d3d_transform_add_rotation_y(double angle)
{

}
void d3d_transform_add_rotation_z(double angle)
{

}
void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, double angle)
{

}

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
	D3DXMATRIX matTranslate;    // a matrix to store the translation information

	// build a matrix to move the model 12 units along the x-axis and 4 units along the y-axis
	// store it to matTranslate
	D3DXMatrixTranslation(&matTranslate, xt, yt, zt);

	// tell Direct3D about our matrix
	d3ddev->SetTransform(D3DTS_WORLD, &matTranslate);
}

void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
	D3DXMATRIX matScale;    // a matrix to store the scaling information

	// build a matrix to double the size of the model
	// store it to matScale
	D3DXMatrixScaling(&matScale, xs, ys, zs);

	// tell Direct3D about our matrix
	d3ddev->SetTransform(D3DTS_WORLD, &matScale);
}

void d3d_transform_set_rotation_x(double angle)
{
	D3DXMATRIX matRotateX;    // a matrix to store the rotation information

	// build a matrix to rotate the model 3.14 radians
	D3DXMatrixRotationX(&matRotateX, angle);

	// tell Direct3D about our matrix
	d3ddev->SetTransform(D3DTS_WORLD, &matRotateX);
}

void d3d_transform_set_rotation_y(double angle)
{
	D3DXMATRIX matRotateY;    // a matrix to store the rotation information

	// build a matrix to rotate the model 3.14 radians
	D3DXMatrixRotationY(&matRotateY, angle);

	// tell Direct3D about our matrix
	d3ddev->SetTransform(D3DTS_WORLD, &matRotateY);
}

void d3d_transform_set_rotation_z(double angle)
{
	D3DXMATRIX matRotateZ;    // a matrix to store the rotation information

	// build a matrix to rotate the model 3.14 radians
	D3DXMatrixRotationX(&matRotateZ, angle);

	// tell Direct3D about our matrix
	d3ddev->SetTransform(D3DTS_WORLD, &matRotateZ);
}

void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, double angle)
{

}

}

#include <stack>
stack<bool> trans_stack;
int trans_stack_size = 0;

namespace enigma_user
{

bool d3d_transform_stack_push()
{

}

bool d3d_transform_stack_pop()
{

}

void d3d_transform_stack_clear()
{

}

bool d3d_transform_stack_empty()
{
    return (trans_stack_size == 0);
}

bool d3d_transform_stack_top()
{

}

bool d3d_transform_stack_disgard()
{
    if (trans_stack_size == 0) return false;
    trans_stack.push(0);
    trans_stack_size--;
    return true;
}

}

// ***** TRANSFORMATIONS END *****

// ***** LIGHTS BEGIN *****
#include <map>
#include <list>
#include "Universal_System/fileio.h"

struct posi { // Homogenous point.
    gs_scalar x;
    gs_scalar y;
    gs_scalar z;
    gs_scalar w;
    posi(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar w1) : x(x1), y(y1), z(z1), w(w1){}
};

class d3d_lights
{
    map<int,int> light_ind;
    map<int,posi> ind_pos; // Internal index to position.

    public:
    d3d_lights() {}
    ~d3d_lights() {}

    void light_update_positions()
    {

    }

    bool light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
    {
		D3DLIGHT9 light;    // create the light struct

		ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
		light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
		light.Diffuse = D3DXCOLOR(__GETR(col), __GETR(col), __GETB(col), 1.0f);    // set the light's color
		light.Direction = D3DXVECTOR3(dx, dy, dz);

		d3ddev->SetLight(id, &light);    // send the light struct properties to nth light 
    }

    bool light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
    {
		D3DLIGHT9 light;    // create the light struct

		ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
		light.Type = D3DLIGHT_POINT;    // make the light type 'directional light'
		light.Diffuse = D3DXCOLOR(__GETR(col), __GETR(col), __GETB(col), 1.0f);    // set the light's color
		light.Position = D3DXVECTOR3(x, y, z);
		light.Range = range;
		light.Attenuation0 = 0.0f;    // no constant inverse attenuation
		light.Attenuation1 = 0.125f;    // only .125 inverse attenuation
		light.Attenuation2 = 0.0f;    // no square inverse attenuation
	    light.Phi = D3DXToRadian(360.0f);    // set the outer cone to 360 degrees
		light.Theta = D3DXToRadian(60.0f);    // set the inner cone to 360 degrees
		light.Falloff = 1.0f;    // use the typical falloff
	
		d3ddev->SetLight(id, &light);    // send the light struct properties to nth light 
    }

    bool light_define_specularity(int id, int r, int g, int b, double a) 
    {

    }

    bool light_enable(int id)
    {
		d3ddev->LightEnable(id, TRUE);    // turn on light #0
    }

    bool light_disable(int id)
    {
		d3ddev->LightEnable(id, FALSE);    // turn on light #0
    }
} d3d_lighting;

namespace enigma_user
{

bool d3d_light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
{
    return d3d_lighting.light_define_direction(id, dx, dy, dz, col);
}

bool d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
{
    return d3d_lighting.light_define_point(id, x, y, z, range, col);
}

bool d3d_light_define_specularity(int id, int r, int g, int b, double a) 
{
    return d3d_lighting.light_define_specularity(id, r, g, b, a);
}

void d3d_light_specularity(int facemode, int r, int g, int b, double a)
{
  float specular[4] = {r, g, b, a};
  
}

void d3d_light_shininess(int facemode, int shine)
{

}

void d3d_light_define_ambient(int col)
{
	//d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(__GETR(col), __GETG(col), __GETB(col)));  
}

bool d3d_light_enable(int id, bool enable)
{
    return enable?d3d_lighting.light_enable(id):d3d_lighting.light_disable(id);
}

}

namespace enigma {
    void d3d_light_update_positions()
    {
        d3d_lighting.light_update_positions();
    }
}

// ***** LIGHTS END *****