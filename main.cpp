// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
// Include GLEW
#include <GL/glew.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif


#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <iostream>
#include <string>
#include <fstream>
#include "Matrix.h"
#include <vector>
#include <thread>

#include "Geometry.h"
#include "Struct.h"
#include "Math.h"


#include "vboindexer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//#define STB_IMAGE_IMPLEMENTATION
//#include"stb_image.h"
#include "shader.h"
#include "camera.h"
#include "OBJ-Loader-master/Source/OBJ_Loader.h"
#include "objloader.hpp"

#include <chrono>
#include "vboindexer.hpp"


// Forward declare ShowFontAtlas() which isn't worth putting in public API yet
namespace ImGui { IMGUI_API void ShowFontAtlas(ImFontAtlas* atlas); }

#define IM_NEWLINE  "\r\n"
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

GLFWwindow* window;
GLFWmonitor* monitor;
const GLFWvidmode* mode;
GLFWvidmode return_struct;

using std::stof;
using std::stoi;

unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);


Matrix MP;
std::vector<glm::vec3> terravertices;
std::vector<glm::vec2> terraUV;
std::vector<Pack_trng> Triangles;
std::vector<Pack_Rod> Rods;
std::vector<Pack_CRod> Crods;
std::vector<Power_line> PL, PS;
//std::vector< STL> STL_obj;
std::vector< border_pack> Borders, Borders2;
bool MCLick = false;
bool ShowSettings = false;

RECT_ RCheck;


object_2d AIM;
object_3d terraObj;
bool BorderShow = true;
std::vector<object_3d> Zones, Rays, Lightings;
IndexObjects IO;
std::vector<Object> Table, TableSelected;
std::vector<BuildingsMesh> BuildingsVector;
std::vector <Mesh_type> RodsVector, CRodsVector, PLVector, ZoneVector, BridgeVector;
std::vector <EBuildingsMesh> EBuildingsMeshVector;
std::vector <TubeMesh> TubeMeshVector;


vector <OnlyBuff> RodsBuff(2), CRodsBuff(3), PLBuff(3), BoxBuff(4), EBuildingsBuff(1), BridgesBuff(1), TubesBuff(1);
/*std::vector<RodMesh> RodsVector;
std::vector<CRodMesh> CRodsVector;
std::vector<PLMesh> PLVector;
std::vector<ZoneMesh> ZoneVector;*/

ObjectInspector Inspector;

//Buffs//
Buffs rodBuffers, CrodBuffers, plBuffers;
//--------------------------------------------//

//B_Textures m_currentMesh = BRICK;
int item_current = 0;
bool SelectColor = false;


float cz = 2.7f;
float cw = 2.7f;

vector <glm::vec3> SelectColors(10);

string path = "Config.txt";
string path_backup = "Config_backup.txt";
string path_textures = "Textures.txt";
ImGuiTheme Theme = DARK;
int ThemeIndex = 0;
int ThemeIndexLast = 0;
float KeyboardSpeed = 100.0f;
float KeyboardSpeedCTRL = 20.0f;

//-------------------------------//
Mode EmptyMode;
vector <Mode> MVector(2, EmptyMode);
Mode* M;
int currentMode, currentModeLast;
int ImportFrom = 0;
bool BindMode = false;
int BindedMode = 0;
int BindedModeLast = 0;
bool BindKey1 = false;
bool BindKey2 = false;
bool BindKey3 = false;
bool BindKey4 = false;
//char* items[] = { "Color", "Concrete", "Brick" };
string items;

//-------------------------------//


glm::vec3 position = glm::vec3(0, 0, 5);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 30.0f; // 3 units / second
float mouseSpeed = 0.005f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int winW = 1600;
int winH = 900;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.5f, 0.5f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 CP;
glm::vec3 CF;
glm::vec3 CU;



GLfloat yaw_main = 45.f;;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch_main = 0.0f;
GLfloat lastX = winW / 2.0;
GLfloat lastY = winH / 2.0;
bool keys[1024];


int PosZone;
int PolygonMode = 0;
int PM = 0;
float min_x, max_X, min_Y, max_Y;
float bmin_x, bmax_X, bmin_Y, bmax_Y;

glm::vec3 Color0, Color7, Color1, Color2, Color3, Color4, Color5, Color6, Color_alt, Color_terra, Color_board;
int ColorMode = 0;
bool SelectedDepth = false;

unsigned int brickTexture, windowTexture, TerraTexture, aimTexture, Concrete, Terra2Txture;
unsigned int brickTexture_normal, Concrete_normal, windowTexture_normal, terra_normal;
unsigned int brickTexture_height, Concrete_height, windowTexture_height;

vector <TextureObject> Textures;


Shader MainShader, Grid, Terra, Board, ShaderMO, FirstLayerShader, BuildingsShader, AimShader, Selected, ColorShader, renderTextureShader,
ColorShaderBuildings, ColorShaderZone, simpleDepthShader, renderTextureDepth, FirstLayerShaderZone;
Shader TerraDepth, BuildingsShaderDepth, ColorShaderDepth, ColorShaderBuildingsDepth;
Shader UnionShader, UnionShaderDepth;

long UID = -1;
long UniqueID() {
    UID++;
    return UID;
}

void genVectors(Mesh_type& m) {
    m.Mesh.reserve(m.n);
    m.Color.reserve(m.n);
    m.ColorSet.reserve(m.n);
    m.ColorMode.reserve(m.n);
    m.ColorModeSet.reserve(m.n);
    for (int i = 0; i < m.n; i++) {
        vector <object_3d*> v;
        m.Mesh.push_back(v);
        vector<glm::vec3> c;
        m.Color.push_back(c);
        m.ColorSet.push_back(c);
        int f = 0;
        vector<int> fc;
        m.ColorMode.push_back(fc);
        m.ColorModeSet.push_back(fc);
    }
};

Cylinder drawCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float R1, float R2, int numv) {
    // Get components of difference vector
    float x = x1 - x2,
        y = y1 - y2,
        z = z1 - z2;
    TVector4f firstPerp = getFirstPerpVector(x, y, z);
    // Get the second perp vector by cross product
    TVector4f secondPerp;
    secondPerp.x = y * firstPerp.z - z * firstPerp.y;
    secondPerp.y = z * firstPerp.x - x * firstPerp.z;
    secondPerp.z = x * firstPerp.y - y * firstPerp.x;
    // Normalize vector
    float length = (secondPerp.x * secondPerp.x) + (secondPerp.y * secondPerp.y) + (secondPerp.z * secondPerp.z);
    length = (float)_CMATH_::sqrt(length);
    secondPerp.x /= length;
    secondPerp.y /= length;
    secondPerp.z /= length;

    // Having now our vectors, here we go:
    // First points; you can have a cone if you change the radius R1

    int ANZ = numv;  // number of vertices
    float FULL = (float)(2.0f * acos(-1));
    //R1 = 20.0f; // radius
    std::vector<TVector4f> points, pointsR2;
    TVector4f P;
    points.reserve(ANZ);
    pointsR2.reserve(ANZ);
    for (int i = 0; i < ANZ; i++) {
        float angle = FULL * (i / (float)ANZ) - 0.785398;
        P.x = (float)(R1 * (cosf(angle) * firstPerp.x + sinf(angle) * secondPerp.x));
        P.y = (float)(R1 * (cosf(angle) * firstPerp.y + sinf(angle) * secondPerp.y));
        P.z = (float)(R1 * (cosf(angle) * firstPerp.z + sinf(angle) * secondPerp.z));
        points.push_back(P);
        P.x = (float)(R2 * (cosf(angle) * firstPerp.x + sinf(angle) * secondPerp.x));
        P.y = (float)(R2 * (cosf(angle) * firstPerp.y + sinf(angle) * secondPerp.y));
        P.z = (float)(R2 * (cosf(angle) * firstPerp.z + sinf(angle) * secondPerp.z));
        pointsR2.push_back(P);
    }
    Cylinder C;
    glm::vec3 vertex1, vertex2, vertex3;
    glm::vec3 vcolor;
    glm::vec3 norm;
    TVector4f p1 = { x1,y1,z1 };
    TVector4f p2 = { x2,y2,z2 };
    TVector4f dir = VectorSubtract(p2, p1);
    TVector4f NormV4 = VectorNormalise(dir);
    norm.x = NormV4.x;
    norm.y = NormV4.y;
    C.vertices.reserve(ANZ * 12);
    C.normals.reserve(ANZ * 12);
    C.colors.reserve(ANZ * 12);
    for (int i = 1; i <= ANZ; i++)
    {
        norm.z = -NormV4.z;
        if (i < ANZ) {
            vertex1.x = x1;
            vertex1.y = y1;
            vertex1.z = z1;
            //obj_stl.v1.x = vertex.x;
            //obj_stl.v1.y = vertex.y;
            //obj_stl.v1.z = vertex.z;
            C.vertices.push_back(vertex1);
            vertex2.x = x1 + points[i - 1].x;
            vertex2.y = y1 + points[i - 1].y;
            vertex2.z = z1 + points[i - 1].z;
            //obj_stl.v2.x = vertex.x;
            //obj_stl.v2.y = vertex.y;
            //obj_stl.v2.z = vertex.z;
            C.vertices.push_back(vertex2);
            vertex3.x = x1 + points[i].x;
            vertex3.y = y1 + points[i].y;
            vertex3.z = z1 + points[i].z;
            //obj_stl.v3.x = vertex.x;
            //obj_stl.v3.y = vertex.y;
            //obj_stl.v3.z = vertex.z;
            C.vertices.push_back(vertex3);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            //obj_stl.norm = norm;
            vcolor.x = 255 / 255.0F;
            vcolor.y = 0;
            vcolor.z = 0;
            C.colors.push_back(vcolor);
            C.colors.push_back(vcolor);
            C.colors.push_back(vcolor);
        }
        else {
            vertex1.x = x1;
            vertex1.y = y1;
            vertex1.z = z1;
            //obj_stl.v1.x = vertex.x;
            //obj_stl.v1.y = vertex.y;
            //obj_stl.v1.z = vertex.z;
            C.vertices.push_back(vertex1);
            vertex2.x = x1 + points[i - 1].x;
            vertex2.y = y1 + points[i - 1].y;
            vertex2.z = z1 + points[i - 1].z;
            //obj_stl.v2.x = vertex.x;
            //obj_stl.v2.y = vertex.y;
            //obj_stl.v2.z = vertex.z;
            C.vertices.push_back(vertex2);
            vertex3.x = x1 + points[0].x;
            vertex3.y = y1 + points[0].y;
            vertex3.z = z1 + points[0].z;
            //obj_stl.v3.x = vertex.x;
            //obj_stl.v3.y = vertex.y;
            //obj_stl.v3.z = vertex.z;
            C.vertices.push_back(vertex3);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            //obj_stl.norm = norm;
            vcolor.x = 255 / 255.0F;
            vcolor.y = 0;
            vcolor.z = 0;
            C.colors.push_back(vcolor);
            C.colors.push_back(vcolor);
            C.colors.push_back(vcolor);
        }
        //STL_obj.push_back(//obj_stl);
    };
    for (int i = 1; i <= ANZ; i++)
    {
        norm.z = NormV4.z;
        if (i < ANZ) {
            vertex1.x = x2;
            vertex1.y = y2;
            vertex1.z = z2;
            //obj_stl.v1.x = vertex.x;
            //obj_stl.v1.y = vertex.y;
            //obj_stl.v1.z = vertex.z;
            C.vertices.push_back(vertex1);
            vertex2.x = x2 + pointsR2[i].x;
            vertex2.y = y2 + pointsR2[i].y;
            vertex2.z = z2 + pointsR2[i].z;
            //obj_stl.v2.x = vertex.x;
            //obj_stl.v2.y = vertex.y;
            //obj_stl.v2.z = vertex.z;
            C.vertices.push_back(vertex2);
            vertex3.x = x2 + pointsR2[i - 1].x;
            vertex3.y = y2 + pointsR2[i - 1].y;
            vertex3.z = z2 + pointsR2[i - 1].z;
            //obj_stl.v3.x = vertex.x;
            //obj_stl.v3.y = vertex.y;
            //obj_stl.v3.z = vertex.z;
            C.vertices.push_back(vertex3);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            //obj_stl.norm = norm;
            vcolor.x = 255 / 255.0F;
            vcolor.y = 0;
            vcolor.z = 0;
            C.colors.push_back(vcolor);
            C.colors.push_back(vcolor);
            C.colors.push_back(vcolor);
        }
        else {
            vertex1.x = x2;
            vertex1.y = y2;
            vertex1.z = z2;
            //obj_stl.v1.x = vertex.x;
            //obj_stl.v1.y = vertex.y;
            //obj_stl.v1.z = vertex.z;
            C.vertices.push_back(vertex1);
            vertex2.x = x2 + pointsR2[0].x;
            vertex2.y = y2 + pointsR2[0].y;
            vertex2.z = z2 + pointsR2[0].z;
            //obj_stl.v2.x = vertex.x;
            //obj_stl.v2.y = vertex.y;
            //obj_stl.v2.z = vertex.z;
            C.vertices.push_back(vertex2);
            vertex3.x = x2 + pointsR2[i - 1].x;
            vertex3.y = y2 + pointsR2[i - 1].y;
            vertex3.z = z2 + pointsR2[i - 1].z;
            //obj_stl.v3.x = vertex.x;
            //obj_stl.v3.y = vertex.y;
            //obj_stl.v3.z = vertex.z;
            C.vertices.push_back(vertex3);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            //obj_stl.norm = norm;
            vcolor.x = 255 / 255.0F;
            vcolor.y = 0;
            vcolor.z = 0;
            C.colors.push_back(vcolor);
            C.colors.push_back(vcolor);
            C.colors.push_back(vcolor);
        }
        //STL_obj.push_back(//obj_stl);
    }
    for (int i = 1; i <= ANZ; i++) {
        if (i < ANZ) {
            vertex1.x = x1 + points[i - 1].x;
            vertex1.y = y1 + points[i - 1].y;
            vertex1.z = z1 + points[i - 1].z;
            //obj_stl.v1.x = vertex.x;
            //obj_stl.v1.y = vertex.y;
            //obj_stl.v1.z = vertex.z;
            C.vertices.push_back(vertex1);
            vertex2.x = x2 + pointsR2[i - 1].x;
            vertex2.y = y2 + pointsR2[i - 1].y;
            vertex2.z = z2 + pointsR2[i - 1].z;
            //obj_stl.v2.x = vertex.x;
            //obj_stl.v2.y = vertex.y;
            //obj_stl.v2.z = vertex.z;
            C.vertices.push_back(vertex2);
            vertex3.x = x2 + pointsR2[i].x;
            vertex3.y = y2 + pointsR2[i].y;
            vertex3.z = z2 + pointsR2[i].z;
            //obj_stl.v3.x = vertex.x;
            //obj_stl.v3.y = vertex.y;
            //obj_stl.v3.z = vertex.z;
            C.vertices.push_back(vertex3);
            TVector4f A, B, K;
            A = { x1 + points[i - 1].x ,y1 + points[i - 1].y,z1 + points[i - 1].z };
            B = { x2 + pointsR2[i - 1].x ,y2 + pointsR2[i - 1].y,z2 + pointsR2[i - 1].z };
            K = { x2 + pointsR2[i].x ,y2 + pointsR2[i].y,z2 + pointsR2[i].z };
            norm = GetNormal(A, B, K);
            //obj_stl.norm = norm;
            //STL_obj.push_back(//obj_stl);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            vertex1.x = x2 + pointsR2[i].x;
            vertex1.y = y2 + pointsR2[i].y;
            vertex1.z = z2 + pointsR2[i].z;
            //obj_stl.v1.x = vertex.x;
            //obj_stl.v1.y = vertex.y;
            //obj_stl.v1.z = vertex.z;
            C.vertices.push_back(vertex1);
            vertex2.x = x1 + points[i].x;
            vertex2.y = y1 + points[i].y;
            vertex2.z = z1 + points[i].z;
            //obj_stl.v2.x = vertex.x;
            //obj_stl.v2.y = vertex.y;
            //obj_stl.v2.z = vertex.z;
            C.vertices.push_back(vertex2);
            vertex3.x = x1 + points[i - 1].x;
            vertex3.y = y1 + points[i - 1].y;
            vertex3.z = z1 + points[i - 1].z;
            //obj_stl.v3.x = vertex.x;
            //obj_stl.v3.y = vertex.y;
            //obj_stl.v3.z = vertex.z;
            C.vertices.push_back(vertex3);
            A = { x2 + pointsR2[i].x ,y2 + pointsR2[i].y,z2 + pointsR2[i].z };
            B = { x1 + points[i].x ,y1 + points[i].y,z1 + points[i].z };
            K = { x1 + points[i - 1].x ,y1 + points[i - 1].y,z1 + points[i - 1].z };
            norm = GetNormal(A, B, K);
            //obj_stl.norm = norm;
            //STL_obj.push_back(//obj_stl);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
        }
        else {
            vertex1.x = x1 + points[i - 1].x;
            vertex1.y = y1 + points[i - 1].y;
            vertex1.z = z1 + points[i - 1].z;
            //obj_stl.v1.x = vertex.x;
            //obj_stl.v1.y = vertex.y;
            //obj_stl.v1.z = vertex.z;
            C.vertices.push_back(vertex1);
            vertex2.x = x2 + pointsR2[i - 1].x;
            vertex2.y = y2 + pointsR2[i - 1].y;
            vertex2.z = z2 + pointsR2[i - 1].z;
            //obj_stl.v2.x = vertex.x;
            //obj_stl.v2.y = vertex.y;
            //obj_stl.v2.z = vertex.z;
            C.vertices.push_back(vertex2);
            vertex3.x = x2 + pointsR2[0].x;
            vertex3.y = y2 + pointsR2[0].y;
            vertex3.z = z2 + pointsR2[0].z;
            //obj_stl.v3.x = vertex.x;
            //obj_stl.v3.y = vertex.y;
            //obj_stl.v3.z = vertex.z;
            C.vertices.push_back(vertex3);
            TVector4f A, B, K;
            A = { x1 + points[i - 1].x ,y1 + points[i - 1].y,z1 + points[i - 1].z };
            B = { x2 + pointsR2[i - 1].x ,y2 + pointsR2[i - 1].y,z2 + pointsR2[i - 1].z };
            K = { x2 + pointsR2[0].x ,y2 + pointsR2[0].y,z2 + pointsR2[0].z };
            norm = GetNormal(A, B, K);
            //obj_stl.norm = norm;
            //STL_obj.push_back(//obj_stl);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            vertex1.x = x2 + pointsR2[0].x;
            vertex1.y = y2 + pointsR2[0].y;
            vertex1.z = z2 + pointsR2[0].z;
            //obj_stl.v1.x = vertex.x;
            //obj_stl.v1.y = vertex.y;
            //obj_stl.v1.z = vertex.z;
            C.vertices.push_back(vertex1);
            vertex2.x = x1 + points[0].x;
            vertex2.y = y1 + points[0].y;
            vertex2.z = z1 + points[0].z;
            //obj_stl.v2.x = vertex.x;
            //obj_stl.v2.y = vertex.y;
            //obj_stl.v2.z = vertex.z;
            C.vertices.push_back(vertex2);
            vertex3.x = x1 + points[i - 1].x;
            vertex3.y = y1 + points[i - 1].y;
            vertex3.z = z1 + points[i - 1].z;
            //obj_stl.v3.x = vertex.x;
            //obj_stl.v3.y = vertex.y;
            //obj_stl.v3.z = vertex.z;
            C.vertices.push_back(vertex3);
            A = { x2 + pointsR2[0].x ,y2 + pointsR2[0].y,z2 + pointsR2[0].z };
            B = { x1 + points[0].x ,y1 + points[0].y,z1 + points[0].z };
            K = { x1 + points[i - 1].x ,y1 + points[i - 1].y,z1 + points[i - 1].z };
            norm = GetNormal(A, B, K);
            //obj_stl.norm = norm;
            //STL_obj.push_back(//obj_stl);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
            C.normals.push_back(norm);
        }
        vcolor.x = 255 / 255.0F;
        vcolor.y = 0;
        vcolor.z = 0;
        C.colors.push_back(vcolor);
        C.colors.push_back(vcolor);
        C.colors.push_back(vcolor);
        C.colors.push_back(vcolor);
        C.colors.push_back(vcolor);
        C.colors.push_back(vcolor);
    }

    return C;
}
CylinderBorder drawCylinderBorder(float x1, float y1, float z1, float x2, float y2, float z2, float R1, float R2, int numv) {
    // Get components of difference vector
    float x = x1 - x2,
        y = y1 - y2,
        z = z1 - z2;
    TVector4f firstPerp = getFirstPerpVector(x, y, z);
    // Get the second perp vector by cross product
    TVector4f secondPerp;
    secondPerp.x = y * firstPerp.z - z * firstPerp.y;
    secondPerp.y = z * firstPerp.x - x * firstPerp.z;
    secondPerp.z = x * firstPerp.y - y * firstPerp.x;
    // Normalize vector
    float length = (secondPerp.x * secondPerp.x) + (secondPerp.y * secondPerp.y) + (secondPerp.z * secondPerp.z);
    length = (float)_CMATH_::sqrt(length);
    secondPerp.x /= length;
    secondPerp.y /= length;
    secondPerp.z /= length;

    // Having now our vectors, here we go:
    // First points; you can have a cone if you change the radius R1

    int ANZ = numv;  // number of vertices
    float FULL = (float)(2.0f * acos(-1));
    //R1 = 20.0f; // radius
    std::vector<TVector4f> points, pointsR2;
    TVector4f P;
    points.reserve(ANZ);
    pointsR2.reserve(ANZ);
    for (int i = 0; i < ANZ; i++) {
        float angle = FULL * (i / (float)ANZ) - 0.785398;
        P.x = (float)(R1 * (cosf(angle) * firstPerp.x + sinf(angle) * secondPerp.x));
        P.y = (float)(R1 * (cosf(angle) * firstPerp.y + sinf(angle) * secondPerp.y));
        P.z = (float)(R1 * (cosf(angle) * firstPerp.z + sinf(angle) * secondPerp.z));
        points.push_back(P);
        P.x = (float)(R2 * (cosf(angle) * firstPerp.x + sinf(angle) * secondPerp.x));
        P.y = (float)(R2 * (cosf(angle) * firstPerp.y + sinf(angle) * secondPerp.y));
        P.z = (float)(R2 * (cosf(angle) * firstPerp.z + sinf(angle) * secondPerp.z));
        pointsR2.push_back(P);
    }
    CylinderBorder C;
    glm::vec3 vertex1, vertex2, vertex3;
    border_pack B;
    C.Border.reserve(3 * ANZ);
    for (int i = 1; i <= ANZ; i++)
    {
        if (i < ANZ) {

            vertex2.x = x1 + points[i - 1].x;
            vertex2.y = y1 + points[i - 1].y;
            vertex2.z = z1 + points[i - 1].z;

            vertex3.x = x1 + points[i].x;
            vertex3.y = y1 + points[i].y;
            vertex3.z = z1 + points[i].z;

            B.x1 = vertex2.x;
            B.y1 = vertex2.y;
            B.z1 = vertex2.z;
            B.x2 = vertex3.x;
            B.y2 = vertex3.y;
            B.z2 = vertex3.z;
            C.Border.push_back(B);

        }
        else {

            vertex2.x = x1 + points[i - 1].x;
            vertex2.y = y1 + points[i - 1].y;
            vertex2.z = z1 + points[i - 1].z;

            vertex3.x = x1 + points[0].x;
            vertex3.y = y1 + points[0].y;
            vertex3.z = z1 + points[0].z;

            B.x1 = vertex2.x;
            B.y1 = vertex2.y;
            B.z1 = vertex2.z;
            B.x2 = vertex3.x;
            B.y2 = vertex3.y;
            B.z2 = vertex3.z;
            C.Border.push_back(B);
        };
    }
    for (int i = 1; i <= ANZ; i++)
    {
        if (i < ANZ) {

            vertex2.x = x2 + pointsR2[i].x;
            vertex2.y = y2 + pointsR2[i].y;
            vertex2.z = z2 + pointsR2[i].z;

            vertex3.x = x2 + pointsR2[i - 1].x;
            vertex3.y = y2 + pointsR2[i - 1].y;
            vertex3.z = z2 + pointsR2[i - 1].z;

            B.x1 = vertex2.x;
            B.y1 = vertex2.y;
            B.z1 = vertex2.z;
            B.x2 = vertex3.x;
            B.y2 = vertex3.y;
            B.z2 = vertex3.z;
            C.Border.push_back(B);
        }
        else {

            vertex2.x = x2 + pointsR2[0].x;
            vertex2.y = y2 + pointsR2[0].y;
            vertex2.z = z2 + pointsR2[0].z;

            vertex3.x = x2 + pointsR2[i - 1].x;
            vertex3.y = y2 + pointsR2[i - 1].y;
            vertex3.z = z2 + pointsR2[i - 1].z;

            B.x1 = vertex2.x;
            B.y1 = vertex2.y;
            B.z1 = vertex2.z;
            B.x2 = vertex3.x;
            B.y2 = vertex3.y;
            B.z2 = vertex3.z;
            C.Border.push_back(B);
        };
        //STL_obj.push_back(//obj_stl);
    }
    for (int i = 1; i <= ANZ; i++) {
        if (i < ANZ) {
            vertex1.x = x1 + points[i - 1].x;
            vertex1.y = y1 + points[i - 1].y;
            vertex1.z = z1 + points[i - 1].z;

            vertex2.x = x2 + pointsR2[i - 1].x;
            vertex2.y = y2 + pointsR2[i - 1].y;
            vertex2.z = z2 + pointsR2[i - 1].z;

            B.x1 = vertex2.x;
            B.y1 = vertex2.y;
            B.z1 = vertex2.z;
            B.x2 = vertex1.x;
            B.y2 = vertex1.y;
            B.z2 = vertex1.z;
            C.Border.push_back(B);
        }
        else {
            vertex1.x = x1 + points[i - 1].x;
            vertex1.y = y1 + points[i - 1].y;
            vertex1.z = z1 + points[i - 1].z;

            vertex2.x = x2 + pointsR2[i - 1].x;
            vertex2.y = y2 + pointsR2[i - 1].y;
            vertex2.z = z2 + pointsR2[i - 1].z;

            B.x1 = vertex2.x;
            B.y1 = vertex2.y;
            B.z1 = vertex2.z;
            B.x2 = vertex1.x;
            B.y2 = vertex1.y;
            B.z2 = vertex1.z;
            C.Border.push_back(B);

        };
    };

    return C;
};


void showObjInspectorZone() {
    ImGui::Begin("Object Inspector Zone");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("Color ZONE");               // Display some text (you can use a format strings too)
        // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SameLine();
    ImGui::Text("   ");
    ImGui::SameLine();
    ImGui::ColorEdit3("Color Rods", (float*)&M->ColorZone, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
    ImGui::Text("");
    ImGui::Separator();
    ImGui::InputFloat("Set Z", (float*)&M->ZoneZ, 0.1f, 1.0f, "%.3f");
    ImGui::Text("");
    ImGui::Separator();
    ImGui::End();
}

void showObjInspectorRods() {
    ImGui::Begin("Object Inspector Rod");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Separator();
    ImGui::Text("Color Rods");               // Display some text (you can use a format strings too)
        // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SameLine();
    ImGui::Text("   ");
    ImGui::SameLine();
    ImGui::ColorEdit3("Color Rods", (float*)&Inspector.Mesh->Color[0][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Text("Color Border");
    ImGui::SameLine();
    ImGui::Text(" ");
    ImGui::SameLine();
    ImGui::ColorEdit3("Color Border", (float*)&Inspector.Mesh->Color[1][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Checkbox("Lock object", &Inspector.Mesh->lock);
    ImGui::Text("");
    ImGui::Separator();
    ImGui::End();
    for (int i = 0; i < Inspector.Mesh->n; i++) {
        if (!AreSameVec3(Inspector.Mesh->ColorSet[i][0], Inspector.Mesh->Color[i][0])) {
            for (int j = 0; j < Inspector.Mesh->Color[i].size(); j++) {
                Inspector.Mesh->Color[i][j] = Inspector.Mesh->Color[i][0];
            }
        }
    }
    if (Inspector.Mesh->lock) {
        for (int i = 0; i < Inspector.Mesh->n; i++) {
            //Inspector.ChangeDataRodColor(i, RodsBuff);
            ChangeColor(i, RodsBuff, Inspector.Mesh);
        }
    }
}

void showObjInspector(const std::string& name, vector <OnlyBuff>& Buff, ObjectInspector& inspector) {
    ImGui::Begin(("Object Inspector " + name).c_str());  // Create a window with the given name
    ImGui::Separator();
    ImGui::Text(("Color " + name).c_str());
    ImGui::SameLine();
    ImGui::Text("   ");
    ImGui::SameLine();
    ImGui::ColorEdit3(("Color " + name).c_str(), (float*)&inspector.Mesh->Color[0][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Checkbox("Lock object", &inspector.Mesh->lock);
    ImGui::Text("");
    ImGui::Separator();
    ImGui::End();

    for (int i = 0; i < inspector.Mesh->n; i++) {
        if (!AreSameVec3(inspector.Mesh->ColorSet[i][0], inspector.Mesh->Color[i][0])) {
            for (int j = 0; j < inspector.Mesh->Color[i].size(); j++) {
                inspector.Mesh->Color[i][j] = inspector.Mesh->Color[i][0];
            }
        }
    }

    if (inspector.Mesh->lock) {
        for (int i = 0; i < inspector.Mesh->n; i++) {
            ChangeColor(i, Buff, inspector.Mesh);
        }
    }
}


void showObjInspectorPL() {
    ImGui::Begin("Object Inspector Power line");                          // Create a window called "Hello, world!" and append into it.                       // Create a window called "Hello, world!" and append into it.
    ImGui::Separator();
    ImGui::Text("Color Rods");               // Display some text (you can use a format strings too)
        // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SameLine();
    ImGui::Text("   ");
    ImGui::SameLine();
    ImGui::ColorEdit3("Color SUPPORTS", (float*)&Inspector.Mesh->Color[0][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Text("Color CABLE");
    ImGui::SameLine();
    ImGui::Text("  ");
    ImGui::SameLine();
    ImGui::ColorEdit3("Color Cable", (float*)&Inspector.Mesh->Color[2][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Text("Color BORDER");
    ImGui::SameLine();
    ImGui::Text(" ");
    ImGui::SameLine();
    ImGui::ColorEdit3("Color Border", (float*)&Inspector.Mesh->Color[1][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Checkbox("Lock object", &Inspector.Mesh->lock);
    ImGui::Text("");
    ImGui::Separator();
    ImGui::End();
    for (int i = 0; i < Inspector.Mesh->n; i++) {
        if (!AreSameVec3(Inspector.Mesh->ColorSet[i][0], Inspector.Mesh->Color[i][0])) {
            for (int j = 0; j < Inspector.Mesh->Color[i].size(); j++) {
                Inspector.Mesh->Color[i][j] = Inspector.Mesh->Color[i][0];
            }
        }
    }
    if (Inspector.Mesh->lock) {
        for (int i = 0; i < Inspector.Mesh->n; i++) {
            //Inspector.ChangeDataRodColor(i, RodsBuff);
            ChangeColor(i, PLBuff, Inspector.Mesh);
        }
    }
}

void showObjInspectorCRods() {
    ImGui::Begin("Object Inspector Cable Rod");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Separator();
    ImGui::Text("Color Rods");               // Display some text (you can use a format strings too)
        // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SameLine();
    ImGui::Text("   ");
    ImGui::SameLine();
    ImGui::ColorEdit3("Color Rods", (float*)&Inspector.Mesh->Color[0][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Text("Color Cable");
    ImGui::SameLine();
    ImGui::Text("  ");
    ImGui::SameLine();
    ImGui::ColorEdit3("Color Cable", (float*)&Inspector.Mesh->Color[2][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Text("Color Border");
    ImGui::SameLine();
    ImGui::Text(" ");
    ImGui::SameLine();
    ImGui::ColorEdit3("Color Border", (float*)&Inspector.Mesh->Color[1][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Checkbox("Lock object", &Inspector.Mesh->lock);
    ImGui::Text("");
    ImGui::Separator();
    ImGui::End();
    for (int i = 0; i < Inspector.Mesh->n; i++) {
        if (!AreSameVec3(Inspector.Mesh->ColorSet[i][0], Inspector.Mesh->Color[i][0])) {
            for (int j = 0; j < Inspector.Mesh->Color[i].size(); j++) {
                Inspector.Mesh->Color[i][j] = Inspector.Mesh->Color[i][0];
            }
        }
    }
    if (Inspector.Mesh->lock) {
        for (int i = 0; i < Inspector.Mesh->n; i++) {
            //Inspector.ChangeDataRodColor(i, RodsBuff);
            ChangeColor(i, CRodsBuff, Inspector.Mesh);
        }
    }
}

void showObjInspectorSky() {

    ImGui::Begin("Object Inspector Sky");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("Pick color");               // Display some text (you can use a format strings too)
        // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("", (float*)&M->ColorSky); // Edit 3 floats representing a color
    ImGui::SameLine(20);
    ImGui::End();
}

void showObjInspectorBuilding() {
    ImGui::Begin("Object Inspector Building");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("Pick texture"); // Display some text (you can use a format strings too)
    //ImGui::SameLine(20);
    ImGui::InputFloat("Floor z", &cz, 0.01f, 1.0f, "%.3f");
    ImGui::InputFloat("Floor w", &cw, 0.01f, 1.0f, "%.3f");
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Text("Box");
    ImGui::Combo("##Box", &Inspector.BMesh->ColorMode[0][0], items.c_str());
    if (Inspector.BMesh->ColorMode[0][0] == 0) {
        ImGui::SameLine();
        ImGui::ColorEdit3("##Color box", (float*)&Inspector.BMesh->Color[0][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    }
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Text("Floor");
    ImGui::Combo("##Floor", &Inspector.BMesh->ColorMode[3][0], items.c_str());
    if (Inspector.BMesh->ColorMode[3][0] == 0) {
        ImGui::SameLine();
        ImGui::ColorEdit3("##Color floor", (float*)&Inspector.BMesh->Color[3][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    }
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Text("Roof");
    ImGui::Combo("##Roof", &Inspector.BMesh->ColorMode[2][0], items.c_str());
    if (Inspector.BMesh->ColorMode[2][0] == 0) {
        ImGui::SameLine();
        ImGui::ColorEdit3("##Color roof", (float*)&Inspector.BMesh->Color[2][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    }
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Text("Border");
    ImGui::Combo("##Border", &Inspector.BMesh->ColorMode[1][0], items.c_str());
    if (Inspector.BMesh->ColorMode[1][0] == 0) {
        ImGui::SameLine();
        ImGui::ColorEdit3("##Color border", (float*)&Inspector.BMesh->Color[1][0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    }
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Checkbox("Lock object", &Inspector.BMesh->lock);
    ImGui::Text("");
    ImGui::Separator();
    // Edit 1 float using a slider from 0.0f to 1.0f
//ImGui::ColorEdit3("clear color", (float*)&Color0); // Edit 3 floats representing a color
//ImGui::SameLine(20);
    ImGui::End();

    for (int i = 0; i < Inspector.BMesh->n; i++) {
        if (!AreSameVec3(Inspector.BMesh->ColorSet[i][0], Inspector.BMesh->Color[i][0])) {
            for (int j = 0; j < Inspector.BMesh->Color[i].size(); j++) {
                Inspector.BMesh->Color[i][j] = Inspector.BMesh->Color[i][0];
            }
        }
    }
    for (int i = 0; i < Inspector.BMesh->n; i++) {
        if (!AreSame(Inspector.BMesh->ColorModeSet[i][0], Inspector.BMesh->ColorMode[i][0])) {
            for (int j = 0; j < Inspector.BMesh->ColorMode[i].size(); j++) {
                Inspector.BMesh->ColorMode[i][j] = Inspector.BMesh->ColorMode[i][0];
            }
        }
    }
    if (Inspector.BMesh->lock) {
        for (int i = 0; i < Inspector.BMesh->n; i++) {
            //Inspector.ChangeDataRodColor(i, RodsBuff);
            ChangeColor(i, BoxBuff, Inspector.BMesh);
            ChangeColorMode(i, BoxBuff, Inspector.BMesh);
        }
    }
}


void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

bool show_options;
bool show_fill_tool;
bool show_selected_tool;
bool show_help;

template < typename T>
void FillBuffFromVector(vector<T>& Vector, vector<OnlyBuff>& buff, int i) {
    for (int k = 0; k < Vector.size(); k++) {
        if (!Vector[k].lock) {
            for (int j = 0; j < Vector[k].Mesh[i].size(); j++) {
                buff[i].fillArr(buff[i].colorbuffer, *buff[i].Color, Vector[k].Mesh[i][j]->p);
            }
        }
    }
};
template < typename T>
void FillBuffFromVector_bool(vector<T>& Vector, vector<OnlyBuff>& buff, int i) {
    float value = (float)*buff[i].ColorMode;
    for (int k = 0; k < Vector.size(); k++) {
        if (!Vector[k].lock) {
            for (int j = 0; j < Vector[k].Mesh[i].size(); j++) {
                buff[i].fillArr(buff[i].boolbuffer, value, Vector[k].Mesh[i][j]->p);
            }
        }
    }
};

void Fill() {
    if (!AreSameVec3(M->ColorEBuilding, M->ColorEBuildingLast)) {
        FillBuffFromVector(EBuildingsMeshVector, EBuildingsBuff, 0);
        M->ColorEBuildingLast.x = M->ColorEBuilding.x;
        M->ColorEBuildingLast.y = M->ColorEBuilding.y;
        M->ColorEBuildingLast.z = M->ColorEBuilding.z;
    }
    if (!AreSameVec3(M->ColorBridge, M->ColorBridgeLast)) {
        FillBuffFromVector(BridgeVector, BridgesBuff, 0);
        M->ColorBridgeLast.x = M->ColorBridge.x;
        M->ColorBridgeLast.y = M->ColorBridge.y;
        M->ColorBridgeLast.z = M->ColorBridge.z;
    }
    if (!AreSameVec3(M->ColorTube, M->ColorTubeLast)) {
        FillBuffFromVector(TubeMeshVector, TubesBuff, 0);
        M->ColorTubeLast.x = M->ColorTube.x;
        M->ColorTubeLast.y = M->ColorTube.y;
        M->ColorTubeLast.z = M->ColorTube.z;
    }
    if (!AreSameVec3(M->ColorRods, M->ColorRodsLast)) {
        FillBuffFromVector(RodsVector, RodsBuff, 0);
        M->ColorRodsLast.x = M->ColorRods.x;
        M->ColorRodsLast.y = M->ColorRods.y;
        M->ColorRodsLast.z = M->ColorRods.z;
    }
    if (!AreSameVec3(M->ColorRodsBorder, M->ColorRodsBorderLast)) {
        FillBuffFromVector(RodsVector, RodsBuff, 1);
        M->ColorRodsBorderLast.x = M->ColorRodsBorder.x;
        M->ColorRodsBorderLast.y = M->ColorRodsBorder.y;
        M->ColorRodsBorderLast.z = M->ColorRodsBorder.z;
    }
    if (!AreSameVec3(M->ColorCRods, M->ColorCRodsLast)) {
        FillBuffFromVector(CRodsVector, CRodsBuff, 0);
        M->ColorCRodsLast.x = M->ColorCRods.x;
        M->ColorCRodsLast.y = M->ColorCRods.y;
        M->ColorCRodsLast.z = M->ColorCRods.z;
    }
    if (!AreSameVec3(M->ColorCRodsBorder, M->ColorCRodsBorderLast)) {
        FillBuffFromVector(CRodsVector, CRodsBuff, 1);
        M->ColorCRodsBorderLast.x = M->ColorCRodsBorder.x;
        M->ColorCRodsBorderLast.y = M->ColorCRodsBorder.y;
        M->ColorCRodsBorderLast.z = M->ColorCRodsBorder.z;
    }
    if (!AreSameVec3(M->ColorCRodsCable, M->ColorCRodsCableLast)) {
        FillBuffFromVector(CRodsVector, CRodsBuff, 2);
        M->ColorCRodsCableLast.x = M->ColorCRodsCable.x;
        M->ColorCRodsCableLast.y = M->ColorCRodsCable.y;
        M->ColorCRodsCableLast.z = M->ColorCRodsCable.z;
    }
    if (!AreSameVec3(M->ColorPL, M->ColorPLLast)) {
        FillBuffFromVector(PLVector, PLBuff, 0);
        M->ColorPLLast.x = M->ColorPL.x;
        M->ColorPLLast.y = M->ColorPL.y;
        M->ColorPLLast.z = M->ColorPL.z;
    }
    if (!AreSameVec3(M->ColorPLBorder, M->ColorPLBorderLast)) {
        FillBuffFromVector(PLVector, PLBuff, 1);
        M->ColorPLBorderLast.x = M->ColorPLBorder.x;
        M->ColorPLBorderLast.y = M->ColorPLBorder.y;
        M->ColorPLBorderLast.z = M->ColorPLBorder.z;
    }
    if (!AreSameVec3(M->ColorPLCable, M->ColorPLCableLast)) {
        FillBuffFromVector(PLVector, PLBuff, 2);
        M->ColorPLCableLast.x = M->ColorPLCable.x;
        M->ColorPLCableLast.y = M->ColorPLCable.y;
        M->ColorPLCableLast.z = M->ColorPLCable.z;
    }
    if (!AreSameVec3(M->ColorBuildingBox, M->ColorBuildingBoxLast)) {
        FillBuffFromVector(BuildingsVector, BoxBuff, 0);
        M->ColorBuildingBoxLast.x = M->ColorBuildingBox.x;
        M->ColorBuildingBoxLast.y = M->ColorBuildingBox.y;
        M->ColorBuildingBoxLast.z = M->ColorBuildingBox.z;
    }
    if (!AreSameVec3(M->ColorBuildingBorder, M->ColorBuildingBorderLast)) {
        FillBuffFromVector(BuildingsVector, BoxBuff, 1);
        M->ColorBuildingBorderLast.x = M->ColorBuildingBorder.x;
        M->ColorBuildingBorderLast.y = M->ColorBuildingBorder.y;
        M->ColorBuildingBorderLast.z = M->ColorBuildingBorder.z;
    }
    if (!AreSameVec3(M->ColorBuildingFloor, M->ColorBuildingFloorLast)) {
        FillBuffFromVector(BuildingsVector, BoxBuff, 2);
        M->ColorBuildingFloorLast.x = M->ColorBuildingFloor.x;
        M->ColorBuildingFloorLast.y = M->ColorBuildingFloor.y;
        M->ColorBuildingFloorLast.z = M->ColorBuildingFloor.z;
    }
    if (!AreSameVec3(M->ColorBuildingRoof, M->ColorBuildingRoofLast)) {
        FillBuffFromVector(BuildingsVector, BoxBuff, 3);
        M->ColorBuildingRoofLast.x = M->ColorBuildingRoof.x;
        M->ColorBuildingRoofLast.y = M->ColorBuildingRoof.y;
        M->ColorBuildingRoofLast.z = M->ColorBuildingRoof.z;
    }
    for (int i = 0; i < M->BuldingsColorMode.size(); i++) {
        if (!AreSame(M->BuldingsColorMode[i], M->BuldingsColorModeLast[i])) {
            FillBuffFromVector_bool(BuildingsVector, BoxBuff, i);
            M->BuldingsColorModeLast[i] = M->BuldingsColorMode[i];
        }
    }
    /*
    if ((M->UseColor_Building != M->UseColor_BuildingLast) || (!AreSameVec3(M->ColorBuildingBox, M->ColorBuildingBoxLast))||(M->item_Building!= M->item_BuildingLast))
    {
        for (int i = 0; i < BuildingsVector.size(); i++) {
            if (!BuildingsVector[i].lock) {

                if (M->UseColor_Building) {

                    BuildingsVector[i].Color[0][0].x = M->ColorBuildingBox.x;
                    BuildingsVector[i].Color[0][0].y = M->ColorBuildingBox.y;
                    BuildingsVector[i].Color[0][0].z = M->ColorBuildingBox.z;
                }
                else {
                    BuildingsVector[i].Wall_texture = static_cast<B_Textures>(M->item_Building);
                    BuildingsVector[i].textureindex = M->item_Building;
                }
                BuildingsVector[i].SelectColor = M->UseColor_Building;
            }
        };
        M->UseColor_BuildingLast = M->UseColor_Building;
        M->ColorBuildingBoxLast.x = M->ColorBuildingBox.x;
        M->ColorBuildingBoxLast.y = M->ColorBuildingBox.y;
        M->ColorBuildingBoxLast.z = M->ColorBuildingBox.z;
        M->item_BuildingLast = M->item_Building;
        cout << "FillBuidlinsSet" << endl;
    }*/
    /*
    if ((UseColor_Building != UseColor_BuildingLast) || ((_CMATH_::abs(ColorBuilding.x - ColorBuildingLast.x) > 0.02) || (_CMATH_::abs(ColorBuilding.y - ColorBuildingLast.y) > 0.02) || (_CMATH_::abs(ColorBuilding.z - ColorBuildingLast.z) > 0.02))
        || (item_Building != item_BuildingLast) || ((_CMATH_::abs(ColorRods.x - ColorRodsLast.x) > 0.02) || (_CMATH_::abs(ColorRods.y - ColorRodsLast.y) > 0.02) || (_CMATH_::abs(ColorRods.z - ColorRodsLast.z) > 0.02))
        || ((_CMATH_::abs(ColorCRods.x - ColorCRodsLast.x) > 0.02) || (_CMATH_::abs(ColorCRods.y - ColorCRodsLast.y) > 0.02) || (_CMATH_::abs(ColorCRods.z - ColorCRodsLast.z) > 0.02))
        || ((_CMATH_::abs(ColorCRodsCable.x - ColorCRodsCableLast.x) > 0.02) || (_CMATH_::abs(ColorCRodsCable.y - ColorCRodsCableLast.y) > 0.02) || (_CMATH_::abs(ColorCRodsCable.z - ColorCRodsCableLast.z) > 0.02))
        || ((_CMATH_::abs(ColorPL.x - ColorPLLast.x) > 0.02) || (_CMATH_::abs(ColorPL.y - ColorPLLast.y) > 0.02) || (_CMATH_::abs(ColorPL.z - ColorPLLast.z) > 0.02))
        || ((_CMATH_::abs(ColorPLCable.x - ColorPLCableLast.x) > 0.02) || (_CMATH_::abs(ColorPLCable.y - ColorPLCableLast.y) > 0.02) || (_CMATH_::abs(ColorPLCable.z - ColorPLCableLast.z) > 0.02))
        || ((_CMATH_::abs(ColorZone.x - ColorZoneLast.x) > 0.02) || (_CMATH_::abs(ColorZone.y - ColorZoneLast.y) > 0.02) || (_CMATH_::abs(ColorZone.z - ColorZoneLast.z) > 0.02)))
    {

        for (int i = 0; i < Table.size(); i++) {
            if (Table[i].b == BUILDING) {
                if (UseColor_Building != UseColor_BuildingLast) {
                    BuildingsVector[Table[i].index].SelectColor = UseColor_Building;
                }
                if ((_CMATH_::abs(ColorBuilding.x - ColorBuildingLast.x) > 0.02) || (_CMATH_::abs(ColorBuilding.y - ColorBuildingLast.y) > 0.02) || (_CMATH_::abs(ColorBuilding.z - ColorBuildingLast.z) > 0.02)) {
                    BuildingsVector[Table[i].index].Color[0][0] = ColorBuilding;
                }
                if (item_Building != item_BuildingLast) {
                    BuildingsVector[Table[i].index].Wall_texture = static_cast<B_Textures>(item_Building);
                }
            }
            if (Table[i].b == RODS) {
                if ((_CMATH_::abs(ColorRods.x - ColorRodsLast.x) > 0.02) || (_CMATH_::abs(ColorRods.y - ColorRodsLast.y) > 0.02) || (_CMATH_::abs(ColorRods.z - ColorRodsLast.z) > 0.02)) {
                    RodsVector[Table[i].index].Color[0][0] = ColorRods;
                }
            }
            if (Table[i].b == CRODS) {
                if ((_CMATH_::abs(ColorCRods.x - ColorCRodsLast.x) > 0.02) || (_CMATH_::abs(ColorCRods.y - ColorCRodsLast.y) > 0.02) || (_CMATH_::abs(ColorCRods.z - ColorCRodsLast.z) > 0.02)) {
                    CRodsVector[Table[i].index].Color[0][0] = ColorCRods;
                }
                if ((_CMATH_::abs(ColorCRodsCable.x - ColorCRodsCableLast.x) > 0.02) || (_CMATH_::abs(ColorCRodsCable.y - ColorCRodsCableLast.y) > 0.02) || (_CMATH_::abs(ColorCRodsCable.z - ColorCRodsCableLast.z) > 0.02)) {
                    CRodsVector[Table[i].index].Color[1][0] = ColorCRodsCable;
                }
            }
            if (Table[i].b == POWERLINE) {
                if ((_CMATH_::abs(ColorPL.x - ColorPLLast.x) > 0.02) || (_CMATH_::abs(ColorPL.y - ColorPLLast.y) > 0.02) || (_CMATH_::abs(ColorPL.z - ColorPLLast.z) > 0.02)) {
                    PLVector[Table[i].index].Color[0][0] = ColorPL;
                }
                if ((_CMATH_::abs(ColorPLCable.x - ColorPLCableLast.x) > 0.02) || (_CMATH_::abs(ColorPLCable.y - ColorPLCableLast.y) > 0.02) || (_CMATH_::abs(ColorPLCable.z - ColorPLCableLast.z) > 0.02)) {
                    PLVector[Table[i].index].Color[1][0] = ColorPLCable;
                }
            }
            if (Table[i].b == ZONE) {
                if ((_CMATH_::abs(ColorZone.x - ColorZoneLast.x) > 0.02) || (_CMATH_::abs(ColorZone.y - ColorZoneLast.y) > 0.02) || (_CMATH_::abs(ColorZone.z - ColorZoneLast.z) > 0.02)) {
                    ZoneVector[Table[i].index].Color[0][0] = ColorZone;
                }
            }
        };
        UseColor_BuildingLast = UseColor_Building;
        ColorBuildingLast.x = ColorBuilding.x;
        ColorBuildingLast.y = ColorBuilding.y;
        ColorBuildingLast.z = ColorBuilding.z;
        item_BuildingLast = item_Building;
        ColorRodsLast.x = ColorRods.x;
        ColorRodsLast.y = ColorRods.y;
        ColorRodsLast.z = ColorRods.z;
        ColorCRodsLast.x = ColorCRods.x;
        ColorCRodsLast.y = ColorCRods.y;
        ColorCRodsLast.z = ColorCRods.z;
        ColorCRodsCableLast.x = ColorCRodsCable.x;
        ColorCRodsCableLast.y = ColorCRodsCable.y;
        ColorCRodsCableLast.z = ColorCRodsCable.z;
        ColorPLLast.x = ColorPL.x;
        ColorPLLast.y = ColorPL.y;
        ColorPLLast.z = ColorPL.z;
        ColorPLCableLast.x = ColorPLCable.x;
        ColorPLCableLast.y = ColorPLCable.y;
        ColorPLCableLast.z = ColorPLCable.z;
        ColorZoneLast.x = ColorZone.x;
        ColorZoneLast.y = ColorZone.y;
        ColorZoneLast.z = ColorZone.z;
    }
    */
}
void SwapMode() {
    RodsBuff[0].Color = &M->ColorRods;
    RodsBuff[1].Color = &M->ColorRodsBorder;
    CRodsBuff[0].Color = &M->ColorCRods;
    CRodsBuff[1].Color = &M->ColorCRodsBorder;
    CRodsBuff[2].Color = &M->ColorCRodsCable;
    PLBuff[0].Color = &M->ColorPL;
    PLBuff[1].Color = &M->ColorPLBorder;
    PLBuff[2].Color = &M->ColorPLCable;
    BoxBuff[0].Color = &M->ColorBuildingBox;
    BoxBuff[1].Color = &M->ColorBuildingBorder;
    BoxBuff[2].Color = &M->ColorBuildingRoof;
    BoxBuff[3].Color = &M->ColorBuildingFloor;
    for (int i = 0; i < BuildingsVector.size(); i++) {
        if (!BuildingsVector[i].lock) {

            if (M->UseColor_Building) {

                BuildingsVector[i].Color[0][0].x = M->ColorBuildingBox.x;
                BuildingsVector[i].Color[0][0].y = M->ColorBuildingBox.y;
                BuildingsVector[i].Color[0][0].z = M->ColorBuildingBox.z;
            }
            else {
                BuildingsVector[i].Wall_texture = static_cast<B_Textures>(M->item_Building);
                BuildingsVector[i].textureindex = M->item_Building;
            }
            BuildingsVector[i].SelectColor = M->UseColor_Building;
        }
    };
    M->UseColor_BuildingLast = M->UseColor_Building;
    M->ColorBuildingBoxLast.x = M->ColorBuildingBox.x;
    M->ColorBuildingBoxLast.y = M->ColorBuildingBox.y;
    M->ColorBuildingBoxLast.z = M->ColorBuildingBox.z;
    M->item_BuildingLast = M->item_Building;

}

void ShowFillTools(bool* p_open)
{
    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
    // Most of the contents of the window will be added by the log.Draw() call.
    ImGui::SetNextWindowSize(ImVec2(winW * 0.4, winH - 150), ImGuiCond_FirstUseEver);
    ImGui::Begin("Tools (Fill)", p_open);
    ImGui::Separator();
    const char* itemsMode[] = { "Mode 0", "Mode 1" };
    ImGui::Combo("##SelectMode", &currentMode, itemsMode, IM_ARRAYSIZE(itemsMode));
    ImGui::Text("");
    ImGui::Text("Import from:");
    ImGui::Combo("##Import from", &ImportFrom, itemsMode, IM_ARRAYSIZE(itemsMode));
    ImGui::SameLine();
    if (ImGui::Button("Import")) {
        *M = MVector[ImportFrom];
        currentModeLast = -1;
    }
    ImGui::Text("");
    ImGui::Separator();
    if (currentMode != currentModeLast) {
        M = &MVector[currentMode];
        currentModeLast = currentMode;
        SwapMode();
    }

    if (ImGui::CollapsingHeader("SKY:")) {
        ImGui::Separator();
        // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("COLOR SKY", (float*)&M->ColorSky, ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
        ImGui::Text("");
        ImGui::Separator();
    }
    if (ImGui::CollapsingHeader("ZONE:")) {
        ImGui::Separator();
        ImGui::Text("Color ZONE");               // Display some text (you can use a format strings too)
        // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::ColorEdit3("Color Zone", (float*)&M->ColorZone, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
        ImGui::Text("");
        ImGui::Separator();
        ImGui::InputFloat("Set Z", (float*)&M->ZoneZ, 0.1f, 1.0f, "%.3f");
        ImGui::Text("");
        ImGui::Separator();
    }
    if (ImGui::CollapsingHeader("BUILDINGS:")) {
        ImGui::Separator();
        ImGui::Text("Box");
        ImGui::Combo("##Box Mode", &M->BuldingsColorMode[0], items.c_str());
        if (M->BuldingsColorMode[0] == 0) {
            ImGui::SameLine();
            ImGui::ColorEdit3("##Color box", (float*)&M->ColorBuildingBox, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        }
        ImGui::Text("");
        ImGui::Separator();
        ImGui::Text("Floor");
        ImGui::Combo("##Floor Mode", &M->BuldingsColorMode[3], items.c_str());
        if (M->BuldingsColorMode[3] == 0) {
            ImGui::SameLine();
            ImGui::ColorEdit3("##Color floor", (float*)&M->ColorBuildingFloor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        }
        ImGui::Text("");
        ImGui::Separator();
        ImGui::Text("Roof");
        ImGui::Combo("##Roof Mode", &M->BuldingsColorMode[2], items.c_str());
        if (M->BuldingsColorMode[2] == 0) {
            ImGui::SameLine();
            ImGui::ColorEdit3("##Color roof", (float*)&M->ColorBuildingRoof, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        }
        ImGui::Text("");
        ImGui::Separator();
        ImGui::Text("Border");
        ImGui::Combo("##Border Mode", &M->BuldingsColorMode[1], items.c_str());
        if (M->BuldingsColorMode[1] == 0) {
            ImGui::SameLine();
            ImGui::ColorEdit3("##Color border", (float*)&M->ColorBuildingBorder, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        }
        ImGui::Text("");
        ImGui::Separator();
    }
    if (ImGui::CollapsingHeader("RODS:")) {
        ImGui::Separator();
        ImGui::Text("Pick color");               // Display some text (you can use a format strings too)
            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SameLine();
        ImGui::Text("     ");
        ImGui::SameLine();
        ImGui::ColorEdit3("Color RODS", (float*)&M->ColorRods, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color

        ImGui::Text("");
        ImGui::Separator();
        ImGui::Text("Color border");               // Display some text (you can use a format strings too)
            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::ColorEdit3("Color border rods", (float*)&M->ColorRodsBorder, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
        ImGui::Text("");
        ImGui::Separator();
    }
    if (ImGui::CollapsingHeader("CABLE RODS:")) {
        ImGui::Separator();
        ImGui::Text("Color rods");               // Display some text (you can use a format strings too)
            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SameLine();
        ImGui::Text("     ");
        ImGui::SameLine();
        ImGui::ColorEdit3("Color CRODS", (float*)&M->ColorCRods, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
        ImGui::Text("");
        ImGui::Separator();
        ImGui::Text("Color cable");               // Display some text (you can use a format strings too)
            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SameLine();
        ImGui::Text("    ");
        ImGui::SameLine();
        ImGui::ColorEdit3("Color cable ", (float*)&M->ColorCRodsCable, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
        ImGui::Text("");
        ImGui::Separator();
        ImGui::Text("Color border");               // Display some text (you can use a format strings too)
            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::ColorEdit3("Color border cable rods", (float*)&M->ColorCRodsBorder, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
        ImGui::Text("");
        ImGui::Separator();
    }
    if (ImGui::CollapsingHeader("POWER LINES:")) {
        ImGui::Separator();
        ImGui::Text("Color SUPPORTS");               // Display some text (you can use a format strings too)
            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SameLine();
        ImGui::Text(" ");
        ImGui::SameLine();
        ImGui::ColorEdit3("Color SUPPORTS", (float*)&M->ColorPL, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
        ImGui::Text("");
        ImGui::Separator();
        ImGui::Text("Color cable");               // Display some text (you can use a format strings too)
            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SameLine();
        ImGui::Text("    ");
        ImGui::SameLine();
        ImGui::ColorEdit3("Color cable PL", (float*)&M->ColorPLCable, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
        ImGui::Text("");
        ImGui::Separator();
        ImGui::Text("Color border");               // Display some text (you can use a format strings too)
            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SameLine();
        ImGui::Text("   ");
        ImGui::SameLine();
        ImGui::ColorEdit3("Color border PL", (float*)&M->ColorPLBorder, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
        ImGui::Text("");
        ImGui::Separator();
    }
    ImGui::Separator();
    ImGui::End();
    Fill();
    // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
}

void ShowSelectedTools(bool* p_open) {
    ImGui::SetNextWindowSize(ImVec2(winW * 0.4, winH - 150), ImGuiCond_FirstUseEver);
    ImGui::Begin("Tools (Shader)", p_open);
    /*ImGui::Separator();
    ImGui::Text("Set Height Scale");
    ImGui::InputFloat("", (float*)&heightScale, 0.01f, 1.0f, "%.3f");
    ImGui::Text("");*/
    ImGui::Separator();
    if (ImGui::CollapsingHeader("Light options:")) {
        ImGui::Separator();
        ImGui::Text("");
        ImGui::Checkbox("Orthographic TERRA", &M->OrthographicTerra);
        if (M->OrthographicTerra) {
            M->OrthographicTerraF = 1.0f;
        }
        else {
            M->OrthographicTerraF = 0.0f;
        }
        ImGui::Checkbox("Orthographic Objects", &M->OrthographicObjects);
        if (M->OrthographicObjects) {
            M->OrthographicObjectsF = 1.0f;
        }
        else {
            M->OrthographicObjectsF = 0.0f;
        }
        ImGui::Checkbox("Orthographic Buildings", &M->OrthographicBuildings);
        if (M->OrthographicBuildings) {
            M->OrthographicBuildingsF = 1.0f;
        }
        else {
            M->OrthographicBuildingsF = 0.0f;
        }
        ImGui::Text("");
        ImGui::Separator();
    }
    ImGui::Checkbox("Enable Shadows", &M->EnableShadows);
    if (M->EnableShadows) {
        ImGui::Text("");
        ImGui::Text("Shadow Scale:");
        ImGui::InputInt("##Shadow Scale", &M->ShadowScale);
        ImGui::Text("");
        ImGui::Text("Shadow Position/Direction");
        if (!M->SetShadowsView) {
            if (ImGui::Button("Change")) {
                M->lightPosLast = M->lightPos;
                M->LightDirectionLast = M->LightDirection;
                M->SetShadowsView = true;

            }
        }
        else
        {
            if (ImGui::Button("Discard")) {
                M->SetShadowsView = false;
                M->lightPos = M->lightPosLast;
                M->LightDirection = M->LightDirectionLast;
                M->MakeShadow = true;
            }
            ImGui::SameLine();
            ImGui::Text("  ");
            ImGui::SameLine();
            if (ImGui::Button("Apply")) {
                M->SetShadowsView = false;
                M->MakeShadow = true;
            }
        }
        ImGui::Text("");
        ImGui::Checkbox("Orthographic shadows mode", &M->OrthographicShadow);
        ImGui::Text("");
        ImGui::Text("Bias");
        if (M->OrthographicShadow) {
            ImGui::InputFloat("##biasORTH", &M->biasOrthographic, 0.0000001f, 0.00001f, "%.8f");
        }
        else {
            ImGui::InputFloat("##biasPERSP", &M->biasPerspective, 0.0000001f, 0.00001f, "%.8f");
        }
    }
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Text("Light Power");
    ImGui::InputFloat("##Light Power", &M->lightPower, 1.0f, 100.0f, "%.3f");
    ImGui::Text("");
    ImGui::Separator();
    ImGui::End();
    Fill();
}

void ShowOptions(bool* p_open, ImGuiStyle* ref)
{
    bool BindModeChange = BindMode;
    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
    // Most of the contents of the window will be added by the log.Draw() call.
    ImGui::SetNextWindowSize(ImVec2(winW * 0.4, winH - 150), ImGuiCond_FirstUseEver);
    ImGui::Begin("Options", p_open);
    ImGui::Separator();
    ImGui::Text("THEME:");
    const char* itemsTheme[] = { "Dark", "Classic", "Light" };
    ImGui::Combo("", &ThemeIndex, itemsTheme, IM_ARRAYSIZE(itemsTheme));
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Text("Speed:");
    ImGui::SliderFloat("##KeyboardSpeed", &KeyboardSpeed, 0.0f, 200.0f);
    ImGui::Text("Speed CTRL:");
    ImGui::SliderFloat("##KeyboardSpeedCTRL", &KeyboardSpeedCTRL, 0.0f, 200.0f);
    if (ThemeIndex != ThemeIndexLast) {
        Theme = static_cast<ImGuiTheme>(ThemeIndex);
        ThemeIndexLast = ThemeIndex;
        if (Theme == DARK) {
            ImGui::StyleColorsDark();
        }
        else if (Theme == CLASSIC) {
            ImGui::StyleColorsClassic();
        }
        else if (Theme == LIGHT) {
            ImGui::StyleColorsLight();
        }

    }
    ImGui::Text("");
    ImGui::Separator();
    ImGui::Checkbox("Bind Mode", &BindMode);
    ImGui::Text("");
    if (BindMode) {
        const char* itemsMode[] = { "Mode 0", "Mode 1" };
        ImGui::Combo("##SelectMode", &BindedMode, itemsMode, IM_ARRAYSIZE(itemsMode));
        ImGui::Text("");
        if (BindModeChange != BindMode) {
            BindedModeLast = currentMode;
        }
        ImGui::Checkbox("Bind key 1", &BindKey1);
        ImGui::Checkbox("Bind key 2", &BindKey2);
        ImGui::Checkbox("Bind key 3", &BindKey3);
        ImGui::Checkbox("Bind key 4", &BindKey4);
    }
    else {
        if (BindModeChange != BindMode) {
            M = &MVector[BindedModeLast];
            SwapMode();
            currentMode = BindedModeLast;
            currentModeLast = BindedModeLast;
        }
    }
    ImGui::Separator();
    ImGui::End();
    // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
}

void ShowHelp(bool* p_open) {
    ImGui::SetNextWindowSize(ImVec2(winW * 0.7, winH - 150), ImGuiCond_FirstUseEver);
    ImGui::Begin("Help", p_open);
    ImGui::Separator();
    ImGui::Text("");
    ImGui::Text("CTRL + ENTER Full Screen Mode");
    ImGui::Text("ALT + ENTER Windowed Mode");
    ImGui::Text("");
    ImGui::Text("1 - The zone becomes a grid");
    ImGui::Text("2 - Solid zone");
    ImGui::Text("3 - The zone becomes dotted");
    ImGui::Text("4 - Hide zone");
    ImGui::Text("");
    ImGui::Text("Q - Remove outline");
    ImGui::Text("E - Show contour");
    ImGui::Text("");
    ImGui::Text("SPACE Display interface and cursor");
    ImGui::Text("");
    ImGui::Text("[Menu File-> Options]");
    ImGui::Text("Speed ​​- movement speed");
    ImGui::Text("SpeedCTRL - movement speed while holding down the CTRL key");
    ImGui::Text("");
    ImGui::Text("[Menu Tools-> Fill tool]");
    ImGui::Text("Global coloring.");
    ImGui::Text("");
    ImGui::Text("[Menu Tools-> Shader tools]");
    ImGui::Text("Lighting setup");
    ImGui::Text("* Light options: - checkboxes change perspective lighting to spelling (like the sun).");
    ImGui::Text("* Enable Shadows - enable shadows.");
    ImGui::Text("Shadow Scale: - the quality of the shadow.");
    ImGui::Text("[Shadow Position / Direction]");
    ImGui::Text("The Change button enables changing the position of the light and shadow.");
    ImGui::Text("With Change pressed");
    ImGui::Text("Discard - discard changes.");
    ImGui::Text("Apply - accept.");
    ImGui::Text("");
    ImGui::Text("Orthographic shadows mode - enable spelling shadows");
    ImGui::Text("");
    ImGui::Text("[Bias]");
    ImGui::Text("Shadow offset. Sometimes some foreshortenings will give shadows not coming from the subject.");
    ImGui::Text("It is necessary to decrease the parameter");
    ImGui::Text("to bring the shadow closer to the subject.");
    ImGui::Text("");
    ImGui::Text("Light Power - the power of light.");
    ImGui::Text("");
    ImGui::Separator();
    ImGui::End();
}

void ShowMenuFile()
{
    //ImGui::MenuItem("(Main menu)", NULL, false, false);
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..")) {}
    ImGui::MenuItem("Options", NULL, &show_options);
    ImGui::Separator();

    // Here we demonstrate appending again to the "Options" menu (which we already created above)
    // Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
    // In a real code-base using it would make senses to use this feature from very different code locations.
    if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

void ShowExampleAppMainMenuBar()
{
    if (show_options)  ShowOptions(&show_options, (ImGuiStyle*)0);
    if (show_fill_tool)  ShowFillTools(&show_fill_tool);
    if (show_selected_tool)  ShowSelectedTools(&show_selected_tool);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Fill tool", NULL, &show_fill_tool);
            ImGui::MenuItem("Shader tools", NULL, &show_selected_tool);
            ImGui::EndMenu();
        }
        ImGui::SameLine();
        std::string text = "Application average %.3f ms/frame (%.1f FPS)";
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text.c_str()).x
            - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::EndMainMenuBar();
    }
}


object_3d MakeTrianglesDynamic(int RowS, int RowE, int ColS, int ColE) {
    float Size = 1;
    glm::vec3 vertex;
    glm::vec3 norm;
    object_3d Zone;
    //Zone.SetIndexAlt(UniqueID());
    //Table.push_back(Object{ Zone.UnIndex ,(unsigned long)Zones.size(),ZONE });
    glm::vec4 P1 = glm::vec4(MP.GetItem(RowS, ColS).x, MP.GetItem(RowS, ColS).y, MP.GetItem(RowS, ColS).z, 1);
    glm::vec4 P2 = glm::vec4(MP.GetItem(RowE, ColS).x, MP.GetItem(RowE, ColS).y, MP.GetItem(RowE, ColS).z, 1);
    glm::vec4 P3 = glm::vec4(MP.GetItem(RowE, ColE).x, MP.GetItem(RowE, ColE).y, MP.GetItem(RowE, ColE).z, 1);
    glm::vec4 P4 = glm::vec4(MP.GetItem(RowS, ColE).x, MP.GetItem(RowS, ColE).y, MP.GetItem(RowS, ColE).z, 1);
    Zone.P1 = P1;
    Zone.P2 = P2;
    Zone.P3 = P3;
    Zone.P4 = P4;
    Zone.vertices.reserve(RowE * ColE * 6);
    Zone.normals.reserve(RowE * ColE * 6);
    //Zone.indices.reserve(RowE * ColE * 6);
    for (int i = RowS; i < RowE; i++)
    {
        for (int j = ColS; j < ColE; j++)
        {
            vertex.x = MP.GetItem(i, j + 1).x / Size;
            vertex.y = MP.GetItem(i, j + 1).y / Size;
            vertex.z = MP.GetItem(i, j + 1).z / Size;
            norm = GetNormal(MP.GetItem(i, j + 1), MP.GetItem(i + 1, j), MP.GetItem(i, j));
            Zone.vertices.push_back(vertex);
            /*if ((i < 7) || (i > MP.GetRow() - 7) || (j < 7) || (j > MP.GetCol() - 7))
            {
                //Zone.colors_backup.push_back(Color7); //Zone.colors_alt.push_back(Color7);
            }
            else {
                //Zone.colors_backup.push_back(Color1); //Zone.colors_alt.push_back(Color_alt);
            }*/
            //Zone.colors_backup.push_back(Color1); //Zone.colors_alt.push_back(Color_alt);
            Zone.normals.push_back(norm);
            if ((i != 0) && (j != 0)) {
                if (MP.GetItem(i, j).w == 0) {
                    MP.Points[i][j].w = Zone.vertices.size() - 1;
                };
            }
            //Zone.indices.push_back(MP.GetItem(i, j).w);
            vertex.x = MP.GetItem(i + 1, j).x / Size;
            vertex.y = MP.GetItem(i + 1, j).y / Size;
            vertex.z = MP.GetItem(i + 1, j).z / Size;
            Zone.vertices.push_back(vertex);
            /*if ((i < 7) || (i > MP.GetRow() - 7) || (j < 7) || (j > MP.GetCol() - 7))
            {
                //Zone.colors_backup.push_back(Color7); //Zone.colors_alt.push_back(Color7);
            }
            else {
                //Zone.colors_backup.push_back(Color1); //Zone.colors_alt.push_back(Color_alt);
            }*/
            //Zone.colors_backup.push_back(Color1); //Zone.colors_alt.push_back(Color_alt);
            Zone.normals.push_back(norm);
            if (MP.GetItem(i + 1, j).w == 0) {
                MP.Points[i + 1][j].w = Zone.vertices.size() - 1;
            }
            //Zone.indices.push_back(MP.GetItem(i + 1, j).w);

            vertex.x = MP.GetItem(i, j).x / Size;
            vertex.y = MP.GetItem(i, j).y / Size;
            vertex.z = MP.GetItem(i, j).z / Size;
            Zone.vertices.push_back(vertex);
            /*if ((i < 7) || (i > MP.GetRow() - 7) || (j < 7) || (j > MP.GetCol() - 7))
            {
                //Zone.colors_backup.push_back(Color7); //Zone.colors_alt.push_back(Color7);
            }
            else {
                //Zone.colors_backup.push_back(Color1); //Zone.colors_alt.push_back(Color_alt);
            }*/
            //Zone.colors_backup.push_back(Color1); //Zone.colors_alt.push_back(Color_alt);
            Zone.normals.push_back(norm);
            if (MP.GetItem(i, j + 1).w == 0) {
                MP.Points[i][j + 1].w = Zone.vertices.size() - 1;
            }
            //Zone.indices.push_back(MP.GetItem(i, j + 1).w);

            norm = GetNormal(MP.GetItem(i, j + 1), MP.GetItem(i + 1, j + 1), MP.GetItem(i + 1, j));
            vertex.x = MP.GetItem(i, j + 1).x / Size;
            vertex.y = MP.GetItem(i, j + 1).y / Size;
            vertex.z = MP.GetItem(i, j + 1).z / Size;
            Zone.vertices.push_back(vertex);
            /*if ((i < 7) || (i > MP.GetRow() - 7) || (j < 7) || (j > MP.GetCol() - 7))
            {
                //Zone.colors_backup.push_back(Color7); //Zone.colors_alt.push_back(Color7);
            }
            else {
                //Zone.colors_backup.push_back(Color1); //Zone.colors_alt.push_back(Color_alt);
            }*/
            //Zone.colors_backup.push_back(Color1); //Zone.colors_alt.push_back(Color_alt);
            Zone.normals.push_back(norm);
            //Zone.indices.push_back(MP.GetItem(i + 1, j).w);
            vertex.x = MP.GetItem(i + 1, j + 1).x / Size;
            vertex.y = MP.GetItem(i + 1, j + 1).y / Size;
            vertex.z = MP.GetItem(i + 1, j + 1).z / Size;
            Zone.vertices.push_back(vertex);
            /*if ((i < 7) || (i > MP.GetRow() - 7) || (j < 7) || (j > MP.GetCol() - 7))
            {
                //Zone.colors_backup.push_back(Color7); //Zone.colors_alt.push_back(Color7);
            }
            else {
                //Zone.colors_backup.push_back(Color1); //Zone.colors_alt.push_back(Color_alt);
            }*/
            //Zone.colors_backup.push_back(Color1); //Zone.colors_alt.push_back(Color_alt);
            Zone.normals.push_back(norm);
            if (MP.GetItem(i + 1, j + 1).w == 0) {
                MP.Points[i + 1][j + 1].w = Zone.vertices.size() - 1;
            }
            //Zone.indices.push_back(MP.GetItem(i + 1, j + 1).w);
            vertex.x = MP.GetItem(i + 1, j).x / Size;
            vertex.y = MP.GetItem(i + 1, j).y / Size;
            vertex.z = MP.GetItem(i + 1, j).z / Size;
            //Zone.indices.push_back(MP.GetItem(i, j + 1).w);
            Zone.vertices.push_back(vertex);
            /*if ((i < 7) || (i > MP.GetRow() - 7) || (j < 7) || (j > MP.GetCol() - 7))
            {
                //Zone.colors_backup.push_back(Color7); //Zone.colors_alt.push_back(Color7);
            }
            else {
                //Zone.colors_backup.push_back(Color1); //Zone.colors_alt.push_back(Color_alt);
            }*/
            //Zone.colors_backup.push_back(Color1); //Zone.colors_alt.push_back(Color_alt);
            Zone.normals.push_back(norm);
        };
    };
    return Zone;
}

object_3d MakeTrianglesDynamic2(int RowS, int RowE, int ColS, int ColE) {
    float Size = 1;
    glm::vec3 vertex;
    glm::vec3 norm, norm2;
    object_3d Zone;
    int t = 0;
    Zone.normals.reserve((RowE + 1) * (ColE + 1));
    Zone.indices.reserve((RowE + 1) * (ColE + 1) * 6);
    for (int i = RowS; i < RowE; i++)
    {
        for (int j = ColS; j < ColE; j++)
        {
            Zone.vertices.push_back(glm::vec3(MP.GetItem(i, j).x, MP.GetItem(i, j).y, MP.GetItem(i, j).z));
        };
    };
    vector<int> a;
    vector<vector<int>> Triangles(Zone.vertices.size(), a);

    for (int i = RowS; i < RowE - 1; i++)
    {
        for (int j = ColS; j < ColE - 1; j++)
        {
            int row1 = i * (ColE);
            int row2 = (i + 1) * (ColE);

            // triangle 1
            Zone.indices.push_back(row1 + j);
            Zone.indices.push_back(row1 + j + 1);
            Zone.indices.push_back(row2 + j + 1);


            // triangle 2
            Zone.indices.push_back(row1 + j);
            Zone.indices.push_back(row2 + j + 1);
            Zone.indices.push_back(row2 + j);

            Triangles[row1 + j].push_back(row1 + j);
            Triangles[row1 + j].push_back(row1 + j + 1);
            Triangles[row1 + j].push_back(row2 + j + 1);
            Triangles[row1 + j + 1].push_back(row1 + j);
            Triangles[row1 + j + 1].push_back(row1 + j + 1);
            Triangles[row1 + j + 1].push_back(row2 + j + 1);
            Triangles[row2 + j + 1].push_back(row1 + j);
            Triangles[row2 + j + 1].push_back(row1 + j + 1);
            Triangles[row2 + j + 1].push_back(row2 + j + 1);
            Triangles[row1 + j].push_back(row1 + j);
            Triangles[row1 + j].push_back(row2 + j + 1);
            Triangles[row1 + j].push_back(row2 + j);
            Triangles[row2 + j + 1].push_back(row1 + j);
            Triangles[row2 + j + 1].push_back(row2 + j + 1);
            Triangles[row2 + j + 1].push_back(row2 + j);
            Triangles[row2 + j].push_back(row1 + j);
            Triangles[row2 + j].push_back(row2 + j + 1);
            Triangles[row2 + j].push_back(row2 + j);
        };
    };
    for (int i = 0; i < Zone.vertices.size(); i++) {
        norm = { 0,0,0 };
        for (int j = 0; j < Triangles[i].size(); j += 3)
        {
            norm += GetNormalNonNormalize(Zone.vertices[Triangles[i][j]], Zone.vertices[Triangles[i][j + 1]], Zone.vertices[Triangles[i][j + 2]]);
        }
        norm = normalize(norm);
        Zone.normals.push_back(norm);
    }
    return Zone;
}

object_3dtest GetWindows2(int i, double coefz, double coefw) {
    float minz = 0;
    double zfloors;
    double zfloors_;
    double widthfloor;
    double widthfloor_;
    glm::vec3 vertex1, vertex2, vertex3;
    object_3dtest Windows;
    glm::vec3 norm;
    glm::vec2 uv1;
    glm::vec2 uv2;
    glm::vec2 uv3;
    glm::vec3 edge1;
    glm::vec3 edge2;
    glm::vec2 deltaUV1;
    glm::vec2 deltaUV2;
    glm::vec3 tangent, bitangent;
    float f;


    for (int j = 1; j < BuildingsVector[i].Points.size(); j++)
    {
        if (BuildingsVector[i].Points[j - 1].z < BuildingsVector[i].Points[j].z) {
            minz = BuildingsVector[i].Points[j - 1].z;
        }
        else {
            minz = BuildingsVector[i].Points[j].z;
        }

        zfloors = minz / coefz;
        zfloors_ = _CMATH_::floor(zfloors);
        zfloors = (zfloors - zfloors_) * coefz;
        widthfloor = (distance(vec2(BuildingsVector[i].Points[j - 1].x, BuildingsVector[i].Points[j - 1].y), vec2(
            BuildingsVector[i].Points[j].x, BuildingsVector[i].Points[j].y))) / coefw;
        widthfloor_ = _CMATH_::floor(widthfloor);
        widthfloor = (widthfloor - widthfloor_) * coefw;

        if ((widthfloor_ > 0) && (zfloors_ > 0)) {
            TVector4f Napr = VectorNormalise(VectorSubtract(BuildingsVector[i].Points[j], BuildingsVector[i].Points[j - 1]));
            TVector4f Normal = { -Napr.y, Napr.x };
            for (int k = 1; k <= zfloors_; k++) {
                for (int t = 1; t <= widthfloor_; t++) {
                    vertex1 = VectorAdd(BuildingsVector[i].Points[j - 1], VectorScale(Napr, (t - 1) * coefw + widthfloor / 2 + 0.25));
                    vertex1 = VectorAdd(vertex1, VectorScale(Normal, 0.03));
                    vertex1.z = (zfloors / 2) + (k - 1) * coefz + 0.575;
                    vertex2 = VectorAdd(BuildingsVector[i].Points[j - 1], VectorScale(Napr, (t - 1) * coefw + widthfloor / 2 + 0.25));
                    vertex2 = VectorAdd(vertex2, VectorScale(Normal, 0.03));
                    vertex2.z = (zfloors / 2) + k * coefz - 0.575;
                    vertex3 = VectorAdd(BuildingsVector[i].Points[j - 1], VectorScale(Napr, t * coefw + widthfloor / 2 - 0.25));
                    vertex3 = VectorAdd(vertex3, VectorScale(Normal, 0.03));
                    vertex3.z = (zfloors / 2) + k * coefz - 0.575;

                    Windows.addVertices(vertex1);
                    Windows.addVertices(vertex2);
                    Windows.addVertices(vertex3);
                    //Windows.addColors_backup(Color2);
                    //Windows.addColors_backup(Color2);
                    //Windows.addColors_backup(Color2);
                    //Windows.addColors_alt(Color2);
                    //Windows.addColors_alt(Color2);
                    //Windows.addColors_alt(Color2);
                    norm = GetNormal(vertex1, vertex2, vertex3);

                    Windows.addNormals(norm);
                    Windows.addNormals(norm);
                    Windows.addNormals(norm);

                    uv1 = glm::vec2{ 1,1 };
                    uv2 = glm::vec2{ 1, 0 };
                    uv3 = glm::vec2{ 0,0 };

                    Windows.addUv(uv1);
                    Windows.addUv(uv2);
                    Windows.addUv(uv3);

                    edge1 = vertex2 - vertex1;
                    edge2 = vertex3 - vertex1;
                    deltaUV1 = uv2 - uv1;
                    deltaUV2 = uv3 - uv1;
                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangent = glm::normalize(tangent);

                    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangent = glm::normalize(bitangent);
                    Windows.addtangent(tangent);
                    Windows.addtangent(tangent);
                    Windows.addtangent(tangent);
                    Windows.addbitangent(bitangent);
                    Windows.addbitangent(bitangent);
                    Windows.addbitangent(bitangent);

                    vertex1 = VectorAdd(BuildingsVector[i].Points[j - 1], VectorScale(Napr, t * coefw + widthfloor / 2 - 0.25));
                    vertex1 = VectorAdd(vertex1, VectorScale(Normal, 0.03));
                    vertex1.z = (zfloors / 2) + k * coefz - 0.575;
                    vertex2 = VectorAdd(BuildingsVector[i].Points[j - 1], VectorScale(Napr, t * coefw + widthfloor / 2 - 0.25));
                    vertex2 = VectorAdd(vertex2, VectorScale(Normal, 0.03));
                    vertex2.z = (zfloors / 2) + (k - 1) * coefz + 0.575;
                    vertex3 = VectorAdd(BuildingsVector[i].Points[j - 1], VectorScale(Napr, (t - 1) * coefw + widthfloor / 2 + 0.25));
                    vertex3 = VectorAdd(vertex3, VectorScale(Normal, 0.03));
                    vertex3.z = (zfloors / 2) + (k - 1) * coefz + 0.575;

                    Windows.addVertices(vertex1);
                    Windows.addVertices(vertex2);
                    Windows.addVertices(vertex3);
                    //Windows.addColors_backup(Color2);
                    //Windows.addColors_backup(Color2);
                    //Windows.addColors_backup(Color2);
                    //Windows.addColors_alt(Color2);
                    //Windows.addColors_alt(Color2);
                    //Windows.addColors_alt(Color2);
                    norm = GetNormal(vertex1, vertex2, vertex3);

                    Windows.addNormals(norm);
                    Windows.addNormals(norm);
                    Windows.addNormals(norm);

                    uv1 = glm::vec2{ 0,0 };
                    uv2 = glm::vec2{ 0, 1 };
                    uv3 = glm::vec2{ 1,1 };

                    Windows.addUv(uv1);
                    Windows.addUv(uv2);
                    Windows.addUv(uv3);

                    edge1 = vertex2 - vertex1;
                    edge2 = vertex3 - vertex1;
                    deltaUV1 = uv2 - uv1;
                    deltaUV2 = uv3 - uv1;
                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangent = glm::normalize(tangent);

                    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangent = glm::normalize(bitangent);
                    Windows.addtangent(tangent);
                    Windows.addtangent(tangent);
                    Windows.addtangent(tangent);
                    Windows.addbitangent(bitangent);
                    Windows.addbitangent(bitangent);
                    Windows.addbitangent(bitangent);


                    //WindowsPoly.push_back(Windows);
                    //B.Windows.push_back(Windows);
                }
            }

        }

    }
    if (BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].z < BuildingsVector[i].Points[0].z) {
        minz = BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].z;
    }
    else {
        minz = BuildingsVector[i].Points[0].z;
    }
    zfloors = minz / coefz;
    zfloors_ = _CMATH_::floor(zfloors);
    zfloors = (zfloors - zfloors_) * coefz;
    widthfloor = (distance(vec2(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].x, BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].y), vec2(
        BuildingsVector[i].Points[0].x, BuildingsVector[i].Points[0].y))) / coefw;
    widthfloor_ = _CMATH_::floor(widthfloor);
    widthfloor = (widthfloor - widthfloor_) * coefw;

    if ((widthfloor_ > 0) && (zfloors_ > 0)) {
        TVector4f Napr = VectorNormalise(VectorSubtract(BuildingsVector[i].Points[0], BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1]));
        TVector4f Normal = { -Napr.y, Napr.x };
        for (int k = 1; k <= zfloors_; k++) {
            for (int t = 1; t <= widthfloor_; t++) {
                vertex1 = VectorAdd(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], VectorScale(Napr, (t - 1) * coefw + widthfloor / 2 + 0.25));
                vertex1 = VectorAdd(vertex1, VectorScale(Normal, 0.03));
                vertex1.z = (zfloors / 2) + (k - 1) * coefz + 0.575;
                vertex2 = VectorAdd(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], VectorScale(Napr, (t - 1) * coefw + widthfloor / 2 + 0.25));
                vertex2 = VectorAdd(vertex2, VectorScale(Normal, 0.03));
                vertex2.z = (zfloors / 2) + k * coefz - 0.575;
                vertex3 = VectorAdd(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], VectorScale(Napr, t * coefw + widthfloor / 2 - 0.25));
                vertex3 = VectorAdd(vertex3, VectorScale(Normal, 0.03));
                vertex3.z = (zfloors / 2) + k * coefz - 0.575;

                Windows.addVertices(vertex1);
                Windows.addVertices(vertex2);
                Windows.addVertices(vertex3);
                //Windows.addColors_backup(Color2);
                //Windows.addColors_backup(Color2);
                //Windows.addColors_backup(Color2);
                //Windows.addColors_alt(Color2);
                //Windows.addColors_alt(Color2);
                //Windows.addColors_alt(Color2);
                norm = GetNormal(vertex1, vertex2, vertex3);

                Windows.addNormals(norm);
                Windows.addNormals(norm);
                Windows.addNormals(norm);
                uv1 = glm::vec2{ 1,1 };
                uv2 = glm::vec2{ 1, 0 };
                uv3 = glm::vec2{ 0,0 };

                Windows.addUv(uv1);
                Windows.addUv(uv2);
                Windows.addUv(uv3);

                edge1 = vertex2 - vertex1;
                edge2 = vertex3 - vertex1;
                deltaUV1 = uv2 - uv1;
                deltaUV2 = uv3 - uv1;
                f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                tangent = glm::normalize(tangent);

                bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                bitangent = glm::normalize(bitangent);
                Windows.addtangent(tangent);
                Windows.addtangent(tangent);
                Windows.addtangent(tangent);
                Windows.addbitangent(bitangent);
                Windows.addbitangent(bitangent);
                Windows.addbitangent(bitangent);

                vertex1 = VectorAdd(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], VectorScale(Napr, t * coefw + widthfloor / 2 - 0.25));
                vertex1 = VectorAdd(vertex1, VectorScale(Normal, 0.03));
                vertex1.z = (zfloors / 2) + k * coefz - 0.575;
                vertex2 = VectorAdd(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], VectorScale(Napr, t * coefw + widthfloor / 2 - 0.25));
                vertex2 = VectorAdd(vertex2, VectorScale(Normal, 0.03));
                vertex2.z = (zfloors / 2) + (k - 1) * coefz + 0.575;
                vertex3 = VectorAdd(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], VectorScale(Napr, (t - 1) * coefw + widthfloor / 2 + 0.25));
                vertex3 = VectorAdd(vertex3, VectorScale(Normal, 0.03));
                vertex3.z = (zfloors / 2) + (k - 1) * coefz + 0.575;

                Windows.addVertices(vertex1);
                Windows.addVertices(vertex2);
                Windows.addVertices(vertex3);
                //Windows.addColors_backup(Color2);
                //Windows.addColors_backup(Color2);
                //Windows.addColors_backup(Color2);
                //Windows.addColors_alt(Color2);
                //Windows.addColors_alt(Color2);
                //Windows.addColors_alt(Color2);
                norm = GetNormal(vertex1, vertex2, vertex3);

                Windows.addNormals(norm);
                Windows.addNormals(norm);
                Windows.addNormals(norm);
                uv1 = glm::vec2{ 0,0 };
                uv2 = glm::vec2{ 0, 1 };
                uv3 = glm::vec2{ 1,1 };

                Windows.addUv(uv1);
                Windows.addUv(uv2);
                Windows.addUv(uv3);

                edge1 = vertex2 - vertex1;
                edge2 = vertex3 - vertex1;
                deltaUV1 = uv2 - uv1;
                deltaUV2 = uv3 - uv1;
                f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                tangent = glm::normalize(tangent);

                bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                bitangent = glm::normalize(bitangent);
                Windows.addtangent(tangent);
                Windows.addtangent(tangent);
                Windows.addtangent(tangent);
                Windows.addbitangent(bitangent);
                Windows.addbitangent(bitangent);
                Windows.addbitangent(bitangent);



                //WindowsPoly.push_back(Windows);
                //B.Windows.push_back(Windows);
            }
        }
    }

    return Windows;
}

object_3d GetWindows(int i, double coefz, double coefw) {
    float minz = 0;
    double zfloors;
    double zfloors_;
    double widthfloor;
    double widthfloor_;
    glm::vec3 vertex1, vertex2, vertex3;
    object_3d Windows;
    glm::vec3 norm;

    for (int j = 1; j < BuildingsVector[i].Points.size(); j++)
    {
        if (BuildingsVector[i].Points[j - 1].z < BuildingsVector[i].Points[j].z) {
            minz = BuildingsVector[i].Points[j - 1].z;
        }
        else {
            minz = BuildingsVector[i].Points[j].z;
        }

        zfloors = minz / coefz;
        zfloors_ = _CMATH_::floor(zfloors);
        zfloors = (zfloors - zfloors_) * coefz;
        widthfloor = (distance(vec2(BuildingsVector[i].Points[j - 1].x, BuildingsVector[i].Points[j - 1].y), vec2(
            BuildingsVector[i].Points[j].x, BuildingsVector[i].Points[j].y))) / coefw;
        widthfloor_ = _CMATH_::floor(widthfloor);
        widthfloor = (widthfloor - widthfloor_) * coefw;

        if ((widthfloor_ > 0) && (zfloors_ > 0)) {
            TVector4f Napr = VectorNormalise(VectorSubtract(BuildingsVector[i].Points[j], BuildingsVector[i].Points[j - 1]));
            TVector4f Normal = { -Napr.y, Napr.x };
            for (int k = 1; k <= zfloors_; k++) {
                for (int t = 1; t <= widthfloor_; t++) {
                    vertex1 = VectorAdd(BuildingsVector[i].Points[j - 1], VectorScale(Napr, (t - 1) * coefw + widthfloor / 2 + 0.25));
                    vertex1 = VectorAdd(vertex1, VectorScale(Normal, 0.03));
                    vertex1.z = (zfloors / 2) + (k - 1) * coefz + 0.575;
                    vertex2 = VectorAdd(BuildingsVector[i].Points[j - 1], VectorScale(Napr, (t - 1) * coefw + widthfloor / 2 + 0.25));
                    vertex2 = VectorAdd(vertex2, VectorScale(Normal, 0.03));
                    vertex2.z = (zfloors / 2) + k * coefz - 0.575;
                    vertex3 = VectorAdd(BuildingsVector[i].Points[j - 1], VectorScale(Napr, t * coefw + widthfloor / 2 - 0.25));
                    vertex3 = VectorAdd(vertex3, VectorScale(Normal, 0.03));
                    vertex3.z = (zfloors / 2) + k * coefz - 0.575;

                    Windows.vertices.push_back(vertex1);
                    Windows.vertices.push_back(vertex2);
                    Windows.vertices.push_back(vertex3);
                    Windows.colors_backup.push_back(Color2);
                    Windows.colors_backup.push_back(Color2);
                    Windows.colors_backup.push_back(Color2);
                    Windows.colors_alt.push_back(Color2);
                    Windows.colors_alt.push_back(Color2);
                    Windows.colors_alt.push_back(Color2);
                    norm = GetNormal(vertex1, vertex2, vertex3);

                    Windows.normals.push_back(norm);
                    Windows.normals.push_back(norm);
                    Windows.normals.push_back(norm);
                    Windows.uv.push_back(glm::vec2{ 1,1 });
                    Windows.uv.push_back(glm::vec2{ 1, 0 });
                    Windows.uv.push_back(glm::vec2{ 0,0 });

                    vertex1 = VectorAdd(BuildingsVector[i].Points[j - 1], VectorScale(Napr, t * coefw + widthfloor / 2 - 0.25));
                    vertex1 = VectorAdd(vertex1, VectorScale(Normal, 0.03));
                    vertex1.z = (zfloors / 2) + k * coefz - 0.575;
                    vertex2 = VectorAdd(BuildingsVector[i].Points[j - 1], VectorScale(Napr, t * coefw + widthfloor / 2 - 0.25));
                    vertex2 = VectorAdd(vertex2, VectorScale(Normal, 0.03));
                    vertex2.z = (zfloors / 2) + (k - 1) * coefz + 0.575;
                    vertex3 = VectorAdd(BuildingsVector[i].Points[j - 1], VectorScale(Napr, (t - 1) * coefw + widthfloor / 2 + 0.25));
                    vertex3 = VectorAdd(vertex3, VectorScale(Normal, 0.03));
                    vertex3.z = (zfloors / 2) + (k - 1) * coefz + 0.575;

                    Windows.vertices.push_back(vertex1);
                    Windows.vertices.push_back(vertex2);
                    Windows.vertices.push_back(vertex3);
                    Windows.colors_backup.push_back(Color2);
                    Windows.colors_backup.push_back(Color2);
                    Windows.colors_backup.push_back(Color2);
                    Windows.colors_alt.push_back(Color2);
                    Windows.colors_alt.push_back(Color2);
                    Windows.colors_alt.push_back(Color2);
                    norm = GetNormal(vertex1, vertex2, vertex3);

                    Windows.normals.push_back(norm);
                    Windows.normals.push_back(norm);
                    Windows.normals.push_back(norm);
                    Windows.uv.push_back(glm::vec2{ 0,0 });
                    Windows.uv.push_back(glm::vec2{ 0, 1 });
                    Windows.uv.push_back(glm::vec2{ 1,1 });


                    //WindowsPoly.push_back(Windows);
                    //B.Windows.push_back(Windows);
                }
            }

        }

    }
    if (BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].z < BuildingsVector[i].Points[0].z) {
        minz = BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].z;
    }
    else {
        minz = BuildingsVector[i].Points[0].z;
    }
    zfloors = minz / coefz;
    zfloors_ = _CMATH_::floor(zfloors);
    zfloors = (zfloors - zfloors_) * coefz;
    widthfloor = (distance(vec2(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].x, BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].y), vec2(
        BuildingsVector[i].Points[0].x, BuildingsVector[i].Points[0].y))) / coefw;
    widthfloor_ = _CMATH_::floor(widthfloor);
    widthfloor = (widthfloor - widthfloor_) * coefw;

    if ((widthfloor_ > 0) && (zfloors_ > 0)) {
        TVector4f Napr = VectorNormalise(VectorSubtract(BuildingsVector[i].Points[0], BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1]));
        TVector4f Normal = { -Napr.y, Napr.x };
        for (int k = 1; k <= zfloors_; k++) {
            for (int t = 1; t <= widthfloor_; t++) {
                vertex1 = VectorAdd(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], VectorScale(Napr, (t - 1) * coefw + widthfloor / 2 + 0.25));
                vertex1 = VectorAdd(vertex1, VectorScale(Normal, 0.03));
                vertex1.z = (zfloors / 2) + (k - 1) * coefz + 0.575;
                vertex2 = VectorAdd(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], VectorScale(Napr, (t - 1) * coefw + widthfloor / 2 + 0.25));
                vertex2 = VectorAdd(vertex2, VectorScale(Normal, 0.03));
                vertex2.z = (zfloors / 2) + k * coefz - 0.575;
                vertex3 = VectorAdd(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], VectorScale(Napr, t * coefw + widthfloor / 2 - 0.25));
                vertex3 = VectorAdd(vertex3, VectorScale(Normal, 0.03));
                vertex3.z = (zfloors / 2) + k * coefz - 0.575;

                Windows.vertices.push_back(vertex1);
                Windows.vertices.push_back(vertex2);
                Windows.vertices.push_back(vertex3);
                Windows.colors_backup.push_back(Color2);
                Windows.colors_backup.push_back(Color2);
                Windows.colors_backup.push_back(Color2);
                Windows.colors_alt.push_back(Color2);
                Windows.colors_alt.push_back(Color2);
                Windows.colors_alt.push_back(Color2);
                norm = GetNormal(vertex1, vertex2, vertex3);

                Windows.normals.push_back(norm);
                Windows.normals.push_back(norm);
                Windows.normals.push_back(norm);
                Windows.uv.push_back(glm::vec2{ 1,1 });
                Windows.uv.push_back(glm::vec2{ 1, 0 });
                Windows.uv.push_back(glm::vec2{ 0,0 });

                vertex1 = VectorAdd(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], VectorScale(Napr, t * coefw + widthfloor / 2 - 0.25));
                vertex1 = VectorAdd(vertex1, VectorScale(Normal, 0.03));
                vertex1.z = (zfloors / 2) + k * coefz - 0.575;
                vertex2 = VectorAdd(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], VectorScale(Napr, t * coefw + widthfloor / 2 - 0.25));
                vertex2 = VectorAdd(vertex2, VectorScale(Normal, 0.03));
                vertex2.z = (zfloors / 2) + (k - 1) * coefz + 0.575;
                vertex3 = VectorAdd(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], VectorScale(Napr, (t - 1) * coefw + widthfloor / 2 + 0.25));
                vertex3 = VectorAdd(vertex3, VectorScale(Normal, 0.03));
                vertex3.z = (zfloors / 2) + (k - 1) * coefz + 0.575;

                Windows.vertices.push_back(vertex1);
                Windows.vertices.push_back(vertex2);
                Windows.vertices.push_back(vertex3);
                Windows.colors_backup.push_back(Color2);
                Windows.colors_backup.push_back(Color2);
                Windows.colors_backup.push_back(Color2);
                Windows.colors_alt.push_back(Color2);
                Windows.colors_alt.push_back(Color2);
                Windows.colors_alt.push_back(Color2);
                norm = GetNormal(vertex1, vertex2, vertex3);

                Windows.normals.push_back(norm);
                Windows.normals.push_back(norm);
                Windows.normals.push_back(norm);
                Windows.uv.push_back(glm::vec2{ 0,0 });
                Windows.uv.push_back(glm::vec2{ 0, 1 });
                Windows.uv.push_back(glm::vec2{ 1,1 });


                //WindowsPoly.push_back(Windows);
                //B.Windows.push_back(Windows);
            }
        }
    }

    return Windows;
}

object_3d MakeRod(glm::vec3 P) {
    object_3d RodObj;
    TVector4f p1, p2;
    p1.x = P.x;
    p1.y = P.y;
    p1.z = P.z / 3 * 2 + 0.012;
    p2.x = P.x;
    p2.y = P.y;
    p2.z = P.z;
    Cylinder C = drawCylinder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 0.1, 0.02, 6);
    p1.z = P.z / 3 + 0.012;
    p2.z = P.z / 3 * 2;
    Cylinder Cmid = drawCylinder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 0.15, 0.15, 6);
    p1.z = P.z / 100 + 0.012;
    p2.z = P.z / 3;
    Cylinder Cfloor = drawCylinder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 0.35, 0.35, 6);
    p1.z = 0;
    p2.z = P.z / 100;
    Cylinder Csqr = drawCylinder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 1, 1, 4);

    RodObj.vertices.reserve(C.vertices.size() + Cmid.vertices.size() + Cfloor.vertices.size() + Csqr.vertices.size());
    for (int k = 0; k < C.vertices.size(); k++)
    {
        RodObj.vertices.push_back(C.vertices[k]);
        //colors.push_back(C.colors[i]);
        //RodObj.colors_backup.push_back(Color3); RodObj.colors_alt.push_back(Color3);
        RodObj.normals.push_back(C.normals[k]);
    }
    for (int k = 0; k < Cmid.vertices.size(); k++)
    {
        RodObj.vertices.push_back(Cmid.vertices[k]);
        //colors.push_back(C.colors[i]);
        //RodObj.colors_backup.push_back(Color3); RodObj.colors_alt.push_back(Color3);
        RodObj.normals.push_back(Cmid.normals[k]);
    }
    for (int k = 0; k < Cfloor.vertices.size(); k++)
    {
        RodObj.vertices.push_back(Cfloor.vertices[k]);
        //colors.push_back(C.colors[i]);
        //RodObj.colors_backup.push_back(Color3); RodObj.colors_alt.push_back(Color3);
        RodObj.normals.push_back(Cfloor.normals[k]);
    }
    for (int k = 0; k < Csqr.vertices.size(); k++)
    {
        RodObj.vertices.push_back(Csqr.vertices[k]);
        //colors.push_back(C.colors[i]);
        //RodObj.colors_backup.push_back(Color3); RodObj.colors_alt.push_back(Color3);
        RodObj.normals.push_back(Csqr.normals[k]);
    }
    return RodObj;
}

object_3d MakeBorderRod(glm::vec3 P) {
    object_3d BorderObj;
    TVector4f p1, p2;
    vector<Cylinder> CB;
    vector<CylinderBorder> C;
    C.reserve(4);
    p1.x = P.x;
    p1.y = P.y;
    p1.z = P.z / 3 * 2 + 0.012;
    p2.x = P.x;
    p2.y = P.y;
    p2.z = P.z;
    C.push_back(drawCylinderBorder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 0.1, 0.02, 6));
    p1.z = P.z / 3 + 0.012;
    p2.z = P.z / 3 * 2;
    C.push_back(drawCylinderBorder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 0.15, 0.15, 6));
    p1.z = P.z / 100 + 0.012;
    p2.z = P.z / 3;
    C.push_back(drawCylinderBorder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 0.35, 0.35, 6));
    p1.z = 0;
    p2.z = P.z / 100;
    C.push_back(drawCylinderBorder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 1, 1, 4));
    int count = 0;
    for (int i = 0; i < C.size(); i++)
    {
        count += C[i].Border.size();
    }
    CB.reserve(count);
    for (int i = 0; i < C.size(); i++)
    {
        for (int k = 0; k < C[i].Border.size(); k++) {
            p1.x = C[i].Border[k].x1;
            p1.y = C[i].Border[k].y1;
            p1.z = C[i].Border[k].z1;
            p2.x = C[i].Border[k].x2;
            p2.y = C[i].Border[k].y2;
            p2.z = C[i].Border[k].z2;
            CB.push_back(drawCylinder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 0.009, 0.009, 3));
        }
    }
    count = 0;
    for (int i = 0; i < CB.size(); i++)
    {
        count += CB[i].vertices.size();
    }
    BorderObj.vertices.reserve(count);
    BorderObj.normals.reserve(count);
    for (int i = 0; i < CB.size(); i++)
    {
        for (int k = 0; k < CB[i].vertices.size(); k++) {
            BorderObj.vertices.push_back(CB[i].vertices[k]);
            BorderObj.normals.push_back(CB[i].normals[k]);
        }
    }

    return BorderObj;
}

object_3d MakeCylinder(glm::vec3 P, double r1, double r2, int parts) {
    object_3d Obj;
    Cylinder C = drawCylinder(P.x, P.y, 0, P.x, P.y, P.z, r1, r2, parts);
    Obj.vertices.reserve(C.vertices.size());
    Obj.normals.reserve(C.vertices.size());
    for (int k = 0; k < C.vertices.size(); k++)
    {
        Obj.vertices.push_back(C.vertices[k]);
        //colors.push_back(C.colors[i]);
        //Obj.colors_backup.push_back(Color3); Obj.colors_alt.push_back(Color3);
        Obj.normals.push_back(C.normals[k]);
        //Obj.indices.push_back(k);
    }
    return Obj;
}

object_3d MakeCylinder(glm::vec3 p1, glm::vec3 p2, double r1, double r2, int parts) {
    object_3d Obj;
    Cylinder C = drawCylinder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, r1, r2, parts);
    Obj.vertices.reserve(C.vertices.size());
    Obj.normals.reserve(C.normals.size());
    for (int k = 0; k < C.vertices.size(); k++)
    {
        Obj.vertices.push_back(C.vertices[k]);
        //colors.push_back(C.colors[i]);
        //Obj.colors_backup.push_back(Color3); //Obj.colors_alt.push_back(Color3);
        Obj.normals.push_back(C.normals[k]);
        //Obj.indices.push_back(k);
    }
    return Obj;
}
object_3d MakeCylinderBorder(glm::vec3 P, double r1, double r2, int parts) {
    object_3d BorderObj;
    vector<Cylinder> CB;
    CylinderBorder C = drawCylinderBorder(P.x, P.y, 0, P.x, P.y, P.z, r1, r2, parts);
    CB.reserve(C.Border.size());
    for (int k = 0; k < C.Border.size(); k++)
    {
        TVector4f p1, p2;
        p1.x = C.Border[k].x1;
        p1.y = C.Border[k].y1;
        p1.z = C.Border[k].z1;
        p2.x = C.Border[k].x2;
        p2.y = C.Border[k].y2;
        p2.z = C.Border[k].z2;
        CB.push_back(drawCylinder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 0.009, 0.009, 3));
    }
    int count = 0;
    for (int i = 0; i < CB.size(); i++) {
        count += CB[i].vertices.size();
    }
    BorderObj.vertices.reserve(count);
    BorderObj.normals.reserve(count);
    for (int i = 0; i < CB.size(); i++) {
        for (int k = 0; k < CB[i].vertices.size(); k++)
        {
            BorderObj.vertices.push_back(CB[i].vertices[k]);
            BorderObj.normals.push_back(CB[i].normals[k]);
        }
    }
    return BorderObj;
}

object_3d MakeCylinderBorder(glm::vec3 p1, glm::vec3 p2, double r1, double r2, int parts) {
    object_3d BorderObj;
    CylinderBorder C = drawCylinderBorder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, r1, r2, parts);
    for (int k = 0; k < C.Border.size(); k++)
    {
        TVector4f p1, p2;
        p1.x = C.Border[k].x1;
        p1.y = C.Border[k].y1;
        p1.z = C.Border[k].z1;
        p2.x = C.Border[k].x2;
        p2.y = C.Border[k].y2;
        p2.z = C.Border[k].z2;
        Cylinder CB = drawCylinder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 0.009, 0.009, 3);
        for (int i = 0; i < CB.vertices.size(); i++)
        {
            BorderObj.vertices.push_back(CB.vertices[i]);
            BorderObj.normals.push_back(CB.normals[i]);
        }
    }
    return BorderObj;
}
object_3d MakeBoundBox(glm::vec3 p1, glm::vec3 p2, double width, double z, BuildingsMesh& b)
{
    glm::vec2 uv1;
    glm::vec2 uv2;
    glm::vec2 uv3;
    glm::vec3 edge1;
    glm::vec3 edge2;
    glm::vec2 deltaUV1;
    glm::vec2 deltaUV2;
    glm::vec3 tangent, bitangent;
    float f;
    double max_valuex;
    double d1, d2;
    double scale = 0.5;

    object_3d Box;
    glm::vec3 vertex1, vertex2, vertex3;
    glm::vec3 p10, p20;;
    p10 = { p1.x,p1.y,0 };
    p20 = { p2.x,p2.y,0 };
    TVector4f dir = VectorSubtract(p20, p10);
    TVector4f NormV4 = VectorNormalise(dir);
    glm::vec3 norm;
    norm.x = -NormV4.y * width;
    norm.y = NormV4.x * width;
    norm.z = z;

    vector<glm::vec3> Points;
    Points.reserve(4);
    Points.push_back(p20 + norm);
    Points.push_back(p20);
    Points.push_back(p10);
    Points.push_back(p10 + norm);
    Box.vertices.reserve(2 * 6);
    Box.normals.reserve(2 * 6);
    Box.uv.reserve(2 * 6);
    Box.tangent.reserve(2 * 6);
    Box.bitangent.reserve(2 * 6);

    if (true) {
        vertex1 = { Points[Points.size() - 1].x , Points[Points.size() - 1].y , 0 };
        vertex2 = { Points[Points.size() - 1].x , Points[Points.size() - 1].y , Points[Points.size() - 1].z };
        vertex3 = { Points[0].x , Points[0].y , Points[0].z };
        Box.vertices.push_back(vertex1);
        Box.vertices.push_back(vertex2);
        Box.vertices.push_back(vertex3);
        norm = GetNormal(vertex1, vertex2, vertex3);

        Box.normals.push_back(norm);
        Box.normals.push_back(norm);
        Box.normals.push_back(norm);

        d2 = distance(vec3(vertex2.x, vertex2.y, 0), vec3(vertex3.x, vertex3.y, 0));

        uv1 = glm::vec2{ 0,0 };
        uv2 = glm::vec2{ 0,  vertex2.z * scale };
        uv3 = glm::vec2{ d2 * scale,  vertex3.z * scale };
        Box.uv.push_back(uv1);
        Box.uv.push_back(uv2);
        Box.uv.push_back(uv3);
        edge1 = vertex2 - vertex1;
        edge2 = vertex3 - vertex1;
        deltaUV1 = uv2 - uv1;
        deltaUV2 = uv3 - uv1;
        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);
        Box.tangent.push_back(tangent);
        Box.tangent.push_back(tangent);
        Box.tangent.push_back(tangent);
        Box.bitangent.push_back(bitangent);
        Box.bitangent.push_back(bitangent);
        Box.bitangent.push_back(bitangent);

        //b.Mesh[1].push_back(new object_3d(MakeCylinder(vertex1, 0.009, 0.009, 8)));
        //b.Mesh[1].push_back(new object_3d(MakeCylinder(vertex2, vertex3, 0.009, 0.009, 8)));

        vertex1 = { Points[0].x , Points[0].y , Points[0].z };
        vertex2 = { Points[0].x , Points[0].y , 0 };
        vertex3 = { Points[Points.size() - 1].x , Points[Points.size() - 1].y , 0 };
        //b.Mesh[1].push_back(new object_3d(MakeCylinder(vertex1, vertex2, 0.009, 0.009, 8)));

        Box.vertices.push_back(vertex1);
        Box.vertices.push_back(vertex2);
        Box.vertices.push_back(vertex3);

        norm = GetNormal(vertex1, vertex2, vertex3);

        Box.normals.push_back(norm);
        Box.normals.push_back(norm);
        Box.normals.push_back(norm);

        d2 = distance(vec3(vertex2.x, vertex2.y, 0), vec3(vertex3.x, vertex3.y, 0));
        uv1 = glm::vec2{ d2 * scale,vertex1.z * scale };
        uv2 = glm::vec2{ d2 * scale,0 };
        uv3 = glm::vec2{ 0,0 };

        Box.uv.push_back(uv1);
        Box.uv.push_back(uv2);
        Box.uv.push_back(uv3);
        edge1 = vertex2 - vertex1;
        edge2 = vertex3 - vertex1;
        deltaUV1 = uv2 - uv1;
        deltaUV2 = uv3 - uv1;
        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);
        Box.tangent.push_back(tangent);
        Box.tangent.push_back(tangent);
        Box.tangent.push_back(tangent);
        Box.bitangent.push_back(bitangent);
        Box.bitangent.push_back(bitangent);
        Box.bitangent.push_back(bitangent);
    };
    norm = GetNormal(Points[0], Points[1], Points[2]);
    NormV4 = VectorNormalise(norm);
    if (NormV4.z > 0.1) {
        norm = { 0,0,1 };
        vertex1 = Points[0];
        vertex1.z = z;
        vertex2 = Points[1];
        vertex2.z = z;
        vertex3 = Points[2];
        vertex3.z = z;
        Box.vertices.push_back(vertex1);
        Box.vertices.push_back(vertex2);
        Box.vertices.push_back(vertex3);
        Box.normals.push_back(norm);
        Box.normals.push_back(norm);
        Box.normals.push_back(norm);

        d1 = distance(vec3(vertex1.x, vertex1.y, 0), vec3(vertex3.x, vertex3.y, 0));
        d2 = distance(vec3(vertex2.x, vertex2.y, 0), vec3(vertex3.x, vertex3.y, 0));

        uv1 = glm::vec2{ 0, 0 };
        uv2 = glm::vec2{ 0, 0 };
        uv3 = glm::vec2{ 0, 0 };
        Box.uv.push_back(uv1);
        Box.uv.push_back(uv2);
        Box.uv.push_back(uv3);
        edge1 = vertex2 - vertex1;
        edge2 = vertex3 - vertex1;
        deltaUV1 = uv2 - uv1;
        deltaUV2 = uv3 - uv1;
        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);
        Box.tangent.push_back(tangent);
        Box.tangent.push_back(tangent);
        Box.tangent.push_back(tangent);
        Box.bitangent.push_back(bitangent);
        Box.bitangent.push_back(bitangent);
        Box.bitangent.push_back(bitangent);

        vertex1 = Points[2];
        vertex1.z = z;
        vertex2 = Points[3];
        vertex2.z = z;
        vertex3 = Points[0];
        vertex3.z = z;
        Box.vertices.push_back(vertex1);
        Box.vertices.push_back(vertex2);
        Box.vertices.push_back(vertex3);
        Box.normals.push_back(norm);
        Box.normals.push_back(norm);
        Box.normals.push_back(norm);

        d2 = distance(vec3(vertex2.x, vertex2.y, 0), vec3(vertex3.x, vertex3.y, 0));
        uv1 = glm::vec2{ 0, 0 };
        uv2 = glm::vec2{ 0, 0 };
        uv3 = glm::vec2{ 0, 0 };

        Box.uv.push_back(uv1);
        Box.uv.push_back(uv2);
        Box.uv.push_back(uv3);
        edge1 = vertex2 - vertex1;
        edge2 = vertex3 - vertex1;
        deltaUV1 = uv2 - uv1;
        deltaUV2 = uv3 - uv1;
        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);
        Box.tangent.push_back(tangent);
        Box.tangent.push_back(tangent);
        Box.tangent.push_back(tangent);
        Box.bitangent.push_back(bitangent);
        Box.bitangent.push_back(bitangent);
        Box.bitangent.push_back(bitangent);
    }
    else {
        //norm = GetNormal(Points[2], Points[1], Points[0]);
        norm = { 0,0,1 };
        vertex1 = Points[2];
        vertex1.z = z;
        vertex2 = Points[1];
        vertex2.z = z;
        vertex3 = Points[0];
        vertex3.z = z;
        Box.vertices.push_back(vertex1);
        Box.vertices.push_back(vertex2);
        Box.vertices.push_back(vertex3);
        Box.normals.push_back(norm);
        Box.normals.push_back(norm);
        Box.normals.push_back(norm);

        d1 = distance(vec3(vertex1.x, vertex1.y, 0), vec3(vertex3.x, vertex3.y, 0));
        d2 = distance(vec3(vertex2.x, vertex2.y, 0), vec3(vertex3.x, vertex3.y, 0));

        uv1 = glm::vec2{ 0, d2 * scale };
        uv2 = glm::vec2{ d1 * scale, d2 * scale };
        uv3 = glm::vec2{ d1 * scale, 0 };
        Box.uv.push_back(uv1);
        Box.uv.push_back(uv2);
        Box.uv.push_back(uv3);
        edge1 = vertex2 - vertex1;
        edge2 = vertex3 - vertex1;
        deltaUV1 = uv2 - uv1;
        deltaUV2 = uv3 - uv1;
        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);
        Box.tangent.push_back(tangent);
        Box.tangent.push_back(tangent);
        Box.tangent.push_back(tangent);
        Box.bitangent.push_back(bitangent);
        Box.bitangent.push_back(bitangent);
        Box.bitangent.push_back(bitangent);

        vertex1 = Points[0];
        vertex1.z = z;
        vertex2 = Points[3];
        vertex2.z = z;
        vertex3 = Points[2];
        vertex3.z = z;
        Box.vertices.push_back(vertex1);
        Box.vertices.push_back(vertex2);
        Box.vertices.push_back(vertex3);
        Box.normals.push_back(norm);
        Box.normals.push_back(norm);
        Box.normals.push_back(norm);

        d1 = distance(vec3(vertex1.x, vertex1.y, 0), vec3(vertex3.x, vertex3.y, 0));
        d2 = distance(vec3(vertex2.x, vertex2.y, 0), vec3(vertex3.x, vertex3.y, 0));
        uv1 = glm::vec2{ d1 * scale, 0 };
        uv2 = glm::vec2{ 0, 0 };
        uv3 = glm::vec2{ 0, d2 * scale };

        Box.uv.push_back(uv1);
        Box.uv.push_back(uv2);
        Box.uv.push_back(uv3);
        edge1 = vertex2 - vertex1;
        edge2 = vertex3 - vertex1;
        deltaUV1 = uv2 - uv1;
        deltaUV2 = uv3 - uv1;
        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);
        Box.tangent.push_back(tangent);
        Box.tangent.push_back(tangent);
        Box.tangent.push_back(tangent);
        Box.bitangent.push_back(bitangent);
        Box.bitangent.push_back(bitangent);
        Box.bitangent.push_back(bitangent);
    }
    return Box;
}

object_3d createEllipseObject(const Ebuilding_pack& EB, int cSteps)
{
    object_3d result;

    float a, b, X, y;
    glm::vec2 center;
    std::vector<glm::vec2> Border1;
    std::vector<glm::vec3>& vertices = result.vertices;
    std::vector<glm::vec3>& normals = result.normals;

    a = (EB.x2 - EB.x1) / 2;
    b = (EB.y2 - EB.y1) / 2;

    center.x = (EB.x2 + EB.x1) / 2;
    center.y = (EB.y2 + EB.y1) / 2;

    if (cSteps < 16)
        cSteps = 16;

    Border1.resize(cSteps + 1);

    for (int i2 = 0; i2 <= cSteps; ++i2)
    {
        float angle = i2 * 2 * 3.14159f / cSteps;
        X = cos(angle);
        y = sin(angle);
        Border1[i2] = glm::vec2(center.x + X * a, center.y + y * b);
    }

    glm::vec3 vert1, vert2, vert3, vert4;

    vert1 = glm::vec3(Border1[0].x, Border1[0].y, 0);
    vert2 = glm::vec3(Border1[0].x, Border1[0].y, EB.h);

    for (int i = 1; i < Border1.size(); ++i)
    {
        vert3 = glm::vec3(Border1[i].x, Border1[i].y, EB.h);
        vert4 = glm::vec3(Border1[i].x, Border1[i].y, 0);

        glm::vec3 Norm = -GetNormal(vert3, vert2, vert1);

        vertices.push_back(vert1);
        normals.push_back(Norm);
        vertices.push_back(vert2);
        normals.push_back(Norm);
        vertices.push_back(vert3);
        normals.push_back(Norm);

        vertices.push_back(vert1);
        normals.push_back(Norm);
        vertices.push_back(vert3);
        normals.push_back(Norm);
        vertices.push_back(vert4);
        normals.push_back(Norm);

        vert1 = vert4;
        vert2 = vert3;
    }

    vert1.x = Border1[0].x;
    vert1.y = Border1[0].y;
    vert1.z = EB.h;

    vert2.x = Border1[1].x;
    vert2.y = Border1[1].y;
    vert2.z = EB.h;

    for (int i = 2; i < Border1.size() - 1; i++)
    {
        vert3.x = Border1[i].x;
        vert3.y = Border1[i].y;
        vert3.z = EB.h;

        glm::vec3 Norm = GetNormal(vert3, vert2, vert1);

        result.vertices.push_back(vert3);
        result.normals.push_back(Norm);
        result.vertices.push_back(vert2);
        result.normals.push_back(Norm);
        result.vertices.push_back(vert1);
        result.normals.push_back(Norm);

        vert2.x = vert3.x;
        vert2.y = vert3.y;
        vert2.z = vert3.z;
    }

    return result;
}

object_3d createSphere(const glm::vec3& center, float radius, int segments)
{
    object_3d sphere;

    // Генерируем вершины и нормали для сферы
    for (int i = 0; i <= segments; ++i)
    {
        float theta1 = static_cast<float>(i) / segments * glm::pi<float>();
        float theta2 = static_cast<float>(i + 1) / segments * glm::pi<float>();

        for (int j = 0; j <= segments; ++j)
        {
            float phi1 = static_cast<float>(j) / segments * glm::two_pi<float>();
            float phi2 = static_cast<float>(j + 1) / segments * glm::two_pi<float>();

            // Вершины треугольника 1
            glm::vec3 vertex1 = center + radius * glm::vec3(std::sin(theta1) * std::cos(phi1), std::cos(theta1), std::sin(theta1) * std::sin(phi1));

            // Вершины треугольника 2
            glm::vec3 vertex2 = center + radius * glm::vec3(std::sin(theta1) * std::cos(phi2), std::cos(theta1), std::sin(theta1) * std::sin(phi2));

            // Вершины треугольника 3
            glm::vec3 vertex3 = center + radius * glm::vec3(std::sin(theta2) * std::cos(phi2), std::cos(theta2), std::sin(theta2) * std::sin(phi2));

            // Вершины треугольника 4
            glm::vec3 vertex4 = center + radius * glm::vec3(std::sin(theta2) * std::cos(phi1), std::cos(theta2), std::sin(theta2) * std::sin(phi1));

            // Нормали для треугольников
            glm::vec3 normal1 = glm::normalize(vertex1 - center);
            glm::vec3 normal2 = glm::normalize(vertex2 - center);
            glm::vec3 normal3 = glm::normalize(vertex3 - center);
            glm::vec3 normal4 = glm::normalize(vertex4 - center);

            // Добавляем вершины и нормали в результат
            sphere.vertices.push_back(vertex1);
            sphere.normals.push_back(normal1);
            sphere.vertices.push_back(vertex2);
            sphere.normals.push_back(normal2);
            sphere.vertices.push_back(vertex3);
            sphere.normals.push_back(normal3);

            sphere.vertices.push_back(vertex3);
            sphere.normals.push_back(normal3);
            sphere.vertices.push_back(vertex4);
            sphere.normals.push_back(normal4);
            sphere.vertices.push_back(vertex1);
            sphere.normals.push_back(normal1);
        }
    }

    return sphere;
}

object_3d create3dPipe(const std::vector<glm::vec3>& points, float radius, int numSegments)
{
    object_3d pipe3d;

    int numPoints = points.size();
    
    for (int i = 0; i < numPoints; i++) {
        glm::vec3 startPoint = points[i];
        glm::vec3 endPoint = points[(i + 1) % numPoints]; // Замыкание трубы
        Cylinder C = drawCylinder(startPoint.x, startPoint.y, startPoint.z, endPoint.x, endPoint.y, endPoint.z, radius, radius, numSegments);
        Cylinder Support = drawCylinder(startPoint.x, startPoint.y, 0, startPoint.x, startPoint.y, startPoint.z, radius * 2 / 3, radius * 2 / 3, 4);
        object_3d Sphere = createSphere(startPoint, radius, numSegments);

        pipe3d.vertices.reserve(pipe3d.vertices.size() + C.vertices.size()  + Sphere.vertices.size() + Support.vertices.size());
        std::copy(C.vertices.begin(), C.vertices.end(), std::back_inserter(pipe3d.vertices));
        std::copy(Sphere.vertices.begin(), Sphere.vertices.end(), std::back_inserter(pipe3d.vertices));
        std::copy(Support.vertices.begin(), Support.vertices.end(), std::back_inserter(pipe3d.vertices));

        pipe3d.normals.reserve(pipe3d.normals.size() + C.normals.size()  + Sphere.normals.size() + Support.normals.size());
        std::copy(C.normals.begin(), C.normals.end(), std::back_inserter(pipe3d.normals));
        std::copy(Sphere.normals.begin(), Sphere.normals.end(), std::back_inserter(pipe3d.normals));
        std::copy(Support.normals.begin(), Support.normals.end(), std::back_inserter(pipe3d.normals));
    }

    return pipe3d;
}

object_3d createAnnulus(const Tube_pack& tube, float height , int numSegments)
{
    object_3d annulus;
    float TopInnerRadius = tube.inRadius/2;
    float TopOuterRadius = tube.inRadius;

    float BottomInnerRadius = tube.inRadius/2;
    float BottomOuterRadius = tube.outRadius;

    float angleIncrement = 2.0f * glm::pi<float>() / numSegments;

    vector<glm::vec2> upOuter, lowOuter;
    vector<glm::vec2> upInner, lowInner;
    upOuter.reserve(numSegments);
    lowOuter.reserve(numSegments);
    upInner.reserve(numSegments);
    lowInner.reserve(numSegments);

    // Calculate vertex positions and normals for the upper annulus
    for (int i = 0; i < numSegments; ++i)
    {
        float angle = i * angleIncrement;

        // Calculate vertex positions for inner and outer radii
        glm::vec2 inner = glm::vec2(tube.x, tube.y) + TopInnerRadius * glm::vec2(cos(angle), sin(angle));
        glm::vec2 outer = glm::vec2(tube.x, tube.y) + TopOuterRadius * glm::vec2(cos(angle), sin(angle));

        upInner.push_back(inner);
        upOuter.push_back(outer);

        // Calculate the next angle
        float nextAngle = (i + 1) * angleIncrement;

        // Calculate the vertex positions for the next inner and outer radii
        glm::vec2 innerNext = glm::vec2(tube.x, tube.y) + TopInnerRadius * glm::vec2(cos(nextAngle), sin(nextAngle));
        glm::vec2 outerNext = glm::vec2(tube.x, tube.y) + TopOuterRadius * glm::vec2(cos(nextAngle), sin(nextAngle));

        // Add vertices for the first triangle (in counter-clockwise order)
        annulus.vertices.push_back(glm::vec3(inner, tube.z));
        annulus.vertices.push_back(glm::vec3(outer, tube.z));
        annulus.vertices.push_back(glm::vec3(outerNext, tube.z));

        // Calculate the normal for the first triangle
        //glm::vec3 normal1 = glm::normalize(glm::cross(glm::vec3(outer - inner, height - tube.z), glm::vec3(outerNext - inner, height - tube.z)));
        glm::vec3 normal1 = glm::normalize(glm::vec3(0, 0, 1));

        // Add the normal for each vertex of the first triangle
        annulus.normals.push_back(normal1);
        annulus.normals.push_back(normal1);
        annulus.normals.push_back(normal1);

        // Add vertices for the second triangle (in counter-clockwise order)
        annulus.vertices.push_back(glm::vec3(inner, tube.z));
        annulus.vertices.push_back(glm::vec3(outerNext, tube.z));
        annulus.vertices.push_back(glm::vec3(innerNext, tube.z));

        // Calculate the normal for the second triangle
        //glm::vec3 normal2 = glm::normalize(glm::cross(glm::vec3(innerNext - outerNext, tube.z - tube.z), glm::vec3(inner - outerNext, tube.z - tube.z)));
        glm::vec3 normal2 = glm::normalize(glm::vec3(0, 0, 1));

        // Add the normal for each vertex of the second triangle
        annulus.normals.push_back(normal2);
        annulus.normals.push_back(normal2);
        annulus.normals.push_back(normal2);
    }

    // Calculate vertex positions and normals for the lower annulus
    for (int i = 0; i < numSegments; ++i)
    {
        float angle = i * angleIncrement;

        // Calculate vertex positions for inner and outer radii
        glm::vec2 inner = glm::vec2(tube.x, tube.y) + BottomInnerRadius * glm::vec2(cos(angle), sin(angle));
        glm::vec2 outer = glm::vec2(tube.x, tube.y) + BottomOuterRadius * glm::vec2(cos(angle), sin(angle));

        lowInner.push_back(inner);
        lowOuter.push_back(outer);

        // Calculate the next angle
        float nextAngle = (i + 1) * angleIncrement;

        // Calculate the vertex positions for the next inner and outer radii
        glm::vec2 innerNext = glm::vec2(tube.x, tube.y) + BottomInnerRadius * glm::vec2(cos(nextAngle), sin(nextAngle));
        glm::vec2 outerNext = glm::vec2(tube.x, tube.y) + BottomOuterRadius * glm::vec2(cos(nextAngle), sin(nextAngle));

        // Add vertices for the first triangle (in clockwise order)
        annulus.vertices.push_back(glm::vec3(inner, 0.0f));
        annulus.vertices.push_back(glm::vec3(outerNext, 0.0f));
        annulus.vertices.push_back(glm::vec3(outer, 0.0f));

        // Calculate the normal for the first triangle
        //glm::vec3 normal1 = glm::normalize(glm::cross(glm::vec3(outerNext - inner, 0.0f), glm::vec3(outer - inner, 0.0f)));
        glm::vec3 normal1 = glm::normalize(glm::vec3(0, 0, -1));

        // Add the normal for each vertex of the first triangle
        annulus.normals.push_back(normal1);
        annulus.normals.push_back(normal1);
        annulus.normals.push_back(normal1);

        // Add vertices for the second triangle (in clockwise order)
        annulus.vertices.push_back(glm::vec3(inner, 0.0f));
        annulus.vertices.push_back(glm::vec3(innerNext, 0.0f));
        annulus.vertices.push_back(glm::vec3(outerNext, 0.0f));

        // Calculate the normal for the second triangle
        //glm::vec3 normal2 = glm::normalize(glm::cross(glm::vec3(innerNext - inner, 0.0f), glm::vec3(outerNext - inner, 0.0f)));
        glm::vec3 normal2 = glm::normalize(glm::vec3(0, 0, -1));

        // Add the normal for each vertex of the second triangle
        annulus.normals.push_back(normal2);
        annulus.normals.push_back(normal2);
        annulus.normals.push_back(normal2);
    }

    // Создание сегмента
    for (int i = 0; i < numSegments; i++) {
        // Вершины сегмента
        glm::vec2 up1 = upOuter[i];
        glm::vec2 up2 = upOuter[(i + 1) % numSegments];
        glm::vec2 low1 = lowOuter[i];
        glm::vec2 low2 = lowOuter[(i + 1) % numSegments];

        // Добавление вершин в annulus.vertices
        annulus.vertices.push_back(glm::vec3(up1.x, up1.y, tube.z));
        annulus.vertices.push_back(glm::vec3(low1.x, low1.y, height));
        annulus.vertices.push_back(glm::vec3(up2.x, up2.y, tube.z));

        annulus.vertices.push_back(glm::vec3(up2.x, up2.y, tube.z));
        annulus.vertices.push_back(glm::vec3(low1.x, low1.y, height));
        annulus.vertices.push_back(glm::vec3(low2.x, low2.y, height));

        // Вычисление нормалей
        glm::vec3 normal1 = -glm::normalize(glm::cross(glm::vec3(up2 - up1, 0.0f), glm::vec3(low1 - up1, height - tube.z)));
        glm::vec3 normal2 = -glm::normalize(glm::cross(glm::vec3(up2 - low1, tube.z - height), glm::vec3(low2 - low1, height - tube.z)));

        // Добавление нормалей в annulus.normals
        annulus.normals.push_back(normal1);
        annulus.normals.push_back(normal1);
        annulus.normals.push_back(normal1);

        annulus.normals.push_back(normal1);
        annulus.normals.push_back(normal1);
        annulus.normals.push_back(normal1);
    }
    std::reverse(upInner.begin(), upInner.end());
    std::reverse(lowInner.begin(), lowInner.end());

    // Создание сегмента
    for (int i = 0; i < numSegments; i++) {
        // Вершины сегмента
        glm::vec2 up1 = upInner[i];
        glm::vec2 up2 = upInner[(i + 1) % numSegments];
        glm::vec2 low1 = lowInner[i];
        glm::vec2 low2 = lowInner[(i + 1) % numSegments];

        // Добавление вершин в annulus.vertices
        annulus.vertices.push_back(glm::vec3(up1.x, up1.y, tube.z));
        annulus.vertices.push_back(glm::vec3(low1.x, low1.y, height));
        annulus.vertices.push_back(glm::vec3(up2.x, up2.y, tube.z));

        annulus.vertices.push_back(glm::vec3(up2.x, up2.y, tube.z));
        annulus.vertices.push_back(glm::vec3(low1.x, low1.y, height));
        annulus.vertices.push_back(glm::vec3(low2.x, low2.y, height));

        // Вычисление нормалей
        glm::vec3 normal1 = -glm::normalize(glm::cross(glm::vec3(up2 - up1, 0.0f), glm::vec3(low1 - up1, height - tube.z)));
        glm::vec3 normal2 = -glm::normalize(glm::cross(glm::vec3(up2 - low1, tube.z - height), glm::vec3(low2 - low1, height - tube.z)));

        // Добавление нормалей в annulus.normals
        annulus.normals.push_back(normal1);
        annulus.normals.push_back(normal1);
        annulus.normals.push_back(normal1);

        annulus.normals.push_back(normal2);
        annulus.normals.push_back(normal2);
        annulus.normals.push_back(normal2);
    }

    return annulus;
}



void MakeTubes(int start, int end) {

    for (int i = start; i < end; i++) {
        TubeMeshVector[i].SetIndexAlt(UniqueID());
        Table.push_back(Object{ TubeMeshVector[i].UnIndex ,(unsigned long)i ,TUBE });
        object_3d* Tube;
        TubeMeshVector[i].Mesh[0].reserve(1);
        TubeMeshVector[i].Color[0].reserve(1);
        TubeMeshVector[i].ColorSet[0].reserve(1);
        TubeMeshVector[i].ColorMode[0].reserve(1);
        TubeMeshVector[i].ColorModeSet[0].reserve(1);
        Tube = new object_3d(createAnnulus(TubeMeshVector[i].Points[0], 0, 32));
        TubeMeshVector[i].Mesh[0].push_back(Tube);
        TubeMeshVector[i].Color[0].push_back(glm::vec3(0.5, 0.5, 0.5));
        TubeMeshVector[i].ColorSet[0].push_back(glm::vec3(0, 0, 0));
        TubeMeshVector[i].ColorMode[0].push_back(0);
        TubeMeshVector[i].ColorModeSet[0].push_back(0);
    }
}

void MakeBridges(int start, int end) {
    glm::vec3 vertex1, vertex2, vertex3;
    glm::vec3 norm, norm_a;

    for (int i = start; i < end; i++) {
        BridgeVector[i].SetIndexAlt(UniqueID());
        Table.push_back(Object{ BridgeVector[i].UnIndex ,(unsigned long)i ,BRIDGE });
        object_3d* PipeBridge, Support;
        BridgeVector[i].Mesh[0].reserve(1);
        BridgeVector[i].Color[0].reserve(1);
        BridgeVector[i].ColorSet[0].reserve(1);
        BridgeVector[i].ColorMode[0].reserve(1);
        BridgeVector[i].ColorModeSet[0].reserve(1);
        PipeBridge = new object_3d(create3dPipe(BridgeVector[i].Points, 0.65, 32));
        BridgeVector[i].Mesh[0].push_back(PipeBridge);
        BridgeVector[i].Color[0].push_back(glm::vec3(0.5, 0.5, 0.5));
        BridgeVector[i].ColorSet[0].push_back(glm::vec3(0, 0, 0));
        BridgeVector[i].ColorMode[0].push_back(0);
        BridgeVector[i].ColorModeSet[0].push_back(0);
        /*BridgeVector[i].Mesh[0].reserve(BridgeVector[i].Points.size());
        BridgeVector[i].Color[0].reserve(BridgeVector[i].Points.size());
        BridgeVector[i].ColorSet[0].reserve(BridgeVector[i].Points.size());
        BridgeVector[i].ColorMode[0].reserve(BridgeVector[i].Points.size());
        BridgeVector[i].ColorModeSet[0].reserve(BridgeVector[i].Points.size());
        for (int j = 0; j < BridgeVector[i].Points.size(); j++)
        {
            //PipeBridge = new object_3d(createEllipseObject(BridgeVector[i].Points[j], 4));
            if (PipeBridge->vertices.size() > 0) {
                BridgeVector[i].Mesh[0].push_back(PipeBridge);
                BridgeVector[i].Color[0].push_back(glm::vec3(0.5, 0.5, 0.5));
                BridgeVector[i].ColorSet[0].push_back(glm::vec3(0, 0, 0));
                BridgeVector[i].ColorMode[0].push_back(0);
                BridgeVector[i].ColorModeSet[0].push_back(0);
            }
            else {
                delete PipeBridge;
            };
        };
        */
    }
}

void MakeEBuildings(int start, int end) {
    glm::vec3 vertex1, vertex2, vertex3;
    glm::vec3 norm, norm_a;

    for (int i = start; i < end; i++) {
        EBuildingsMeshVector[i].SetIndexAlt(UniqueID());
        Table.push_back(Object{ EBuildingsMeshVector[i].UnIndex ,(unsigned long)i ,EBUILDING });
        object_3d* EObj;
        EBuildingsMeshVector[i].Mesh[0].reserve(EBuildingsMeshVector[i].Points.size());
        EBuildingsMeshVector[i].Color[0].reserve(EBuildingsMeshVector[i].Points.size());
        EBuildingsMeshVector[i].ColorSet[0].reserve(EBuildingsMeshVector[i].Points.size());
        EBuildingsMeshVector[i].ColorMode[0].reserve(EBuildingsMeshVector[i].Points.size());
        EBuildingsMeshVector[i].ColorModeSet[0].reserve(EBuildingsMeshVector[i].Points.size());
        for (int j = 0; j < EBuildingsMeshVector[i].Points.size(); j++)
        {
            EObj = new object_3d(createEllipseObject(EBuildingsMeshVector[i].Points[j],4));
            if (EObj->vertices.size() > 0) {
                EBuildingsMeshVector[i].Mesh[0].push_back(EObj);
                EBuildingsMeshVector[i].Color[0].push_back(glm::vec3(1, 1, 1));
                EBuildingsMeshVector[i].ColorSet[0].push_back(glm::vec3(0, 0, 0));
                EBuildingsMeshVector[i].ColorMode[0].push_back(0);
                EBuildingsMeshVector[i].ColorModeSet[0].push_back(0);
            }
            else {
                delete EObj;
            };
        };
    }
}

void MakeBuildings(int start, int end) {
    glm::vec3 vertex1, vertex2, vertex3;
    glm::vec3 norm, norm_a;
    glm::vec2 uv1;
    glm::vec2 uv2;
    glm::vec2 uv3;
    glm::vec3 edge1;
    glm::vec3 edge2;
    glm::vec2 deltaUV1;
    glm::vec2 deltaUV2;
    glm::vec3 tangent, bitangent;
    float f;
    double max_valuex;
    double d1, d2;

    for (int i = start; i < end; i++) {
        BuildingsVector[i].SetIndexAlt(UniqueID());
        Table.push_back(Object{ BuildingsVector[i].UnIndex ,(unsigned long)i ,BUILDING });
        object_3d* Box = new object_3d;
        object_3d* Roof = new object_3d;
        //object_3d* Border = new object_3d;
        object_3dtest* Windows;
        vector <glm::vec3> Roofs;
        vector <glm::vec3> base;
        bool isRoof = false;
        int N = 0;
        max_valuex = BuildingsVector[i].Points[0].x;
        double scale = 0.5;
        double baseScale = 1.05;
        double basez = 0.2;


        int zfloors_;
        double zfloors;
        double widthfloor;
        int widthfloor_;
        double minz;

        /// <Box>
        Box->vertices.reserve(BuildingsVector[i].Points.size());
        Box->normals.reserve(BuildingsVector[i].Points.size());
        Box->uv.reserve(BuildingsVector[i].Points.size());
        Box->tangent.reserve(BuildingsVector[i].Points.size());
        Box->bitangent.reserve(BuildingsVector[i].Points.size());
        for (int j = 1; j < BuildingsVector[i].Points.size(); j++)
        {
            BuildingsVector[i].Mesh[3].push_back(new object_3d(MakeBoundBox(BuildingsVector[i].Points[j - 1], BuildingsVector[i].Points[j], 0.4, 0.3, BuildingsVector[i])));
            BuildingsVector[i].Color[3].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[3].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[3].push_back(0);
            BuildingsVector[i].ColorModeSet[3].push_back(0);
            vertex1 = { BuildingsVector[i].Points[j - 1].x , BuildingsVector[i].Points[j - 1].y , 0.299 };
            BuildingsVector[i].Mesh[3].push_back(new object_3d(MakeCylinder(vertex1, 0.4, 0.4, 32)));
            BuildingsVector[i].Color[3].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[3].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[3].push_back(0);
            BuildingsVector[i].ColorModeSet[3].push_back(0);
            vertex1 = { BuildingsVector[i].Points[j - 1].x , BuildingsVector[i].Points[j - 1].y , 0 };
            vertex2 = { BuildingsVector[i].Points[j - 1].x , BuildingsVector[i].Points[j - 1].y , BuildingsVector[i].Points[j - 1].z };
            vertex3 = { BuildingsVector[i].Points[j].x , BuildingsVector[i].Points[j].y , BuildingsVector[i].Points[j].z };
            Box->vertices.push_back(vertex1);
            Box->vertices.push_back(vertex2);
            Box->vertices.push_back(vertex3);
            norm = GetNormal(vertex1, vertex2, vertex3);

            Box->normals.push_back(norm);
            Box->normals.push_back(norm);
            Box->normals.push_back(norm);

            d2 = distance(vec3(vertex2.x, vertex2.y, 0), vec3(vertex3.x, vertex3.y, 0));

            uv1 = glm::vec2{ 0,0 };
            uv2 = glm::vec2{ 0, BuildingsVector[i].Points[j - 1].z * scale };
            uv3 = glm::vec2{ d2 * scale, BuildingsVector[i].Points[j].z * scale };
            Box->uv.push_back(uv1);
            Box->uv.push_back(uv2);
            Box->uv.push_back(uv3);
            edge1 = vertex2 - vertex1;
            edge2 = vertex3 - vertex1;
            deltaUV1 = uv2 - uv1;
            deltaUV2 = uv3 - uv1;
            f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
            bitangent = glm::normalize(bitangent);
            Box->tangent.push_back(tangent);
            Box->tangent.push_back(tangent);
            Box->tangent.push_back(tangent);
            Box->bitangent.push_back(bitangent);
            Box->bitangent.push_back(bitangent);
            Box->bitangent.push_back(bitangent);

            BuildingsVector[i].Mesh[1].push_back(new object_3d(MakeCylinder(vertex2, vertex3, 0.009, 0.009, 3)));
            BuildingsVector[i].Color[1].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[1].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[1].push_back(0);
            BuildingsVector[i].ColorModeSet[1].push_back(0);

            vertex1 = { BuildingsVector[i].Points[j].x , BuildingsVector[i].Points[j].y , BuildingsVector[i].Points[j].z };
            vertex2 = { BuildingsVector[i].Points[j].x , BuildingsVector[i].Points[j].y , 0 };
            vertex3 = { BuildingsVector[i].Points[j - 1].x , BuildingsVector[i].Points[j - 1].y , 0 };



            Box->vertices.push_back(vertex1);
            Box->vertices.push_back(vertex2);
            Box->vertices.push_back(vertex3);
            //NBuilding->colors_backup.push_back(Color2);
            //NBuilding->colors_backup.push_back(Color2);
            //NBuilding->colors_backup.push_back(Color2);
            //NBuilding->colors_alt.push_back(Color2);
            //NBuilding->colors_alt.push_back(Color2);
            //NBuilding->colors_alt.push_back(Color2);
            norm = GetNormal(vertex1, vertex2, vertex3);

            Box->normals.push_back(norm);
            Box->normals.push_back(norm);
            Box->normals.push_back(norm);

            d2 = distance(vec3(vertex2.x, vertex2.y, 0), vec3(vertex3.x, vertex3.y, 0));
            uv1 = glm::vec2{ d2 * scale,BuildingsVector[i].Points[j].z * scale };
            uv2 = glm::vec2{ d2 * scale,0 };
            uv3 = glm::vec2{ 0,0 };

            Box->uv.push_back(uv1);
            Box->uv.push_back(uv2);
            Box->uv.push_back(uv3);
            edge1 = vertex2 - vertex1;
            edge2 = vertex3 - vertex1;
            deltaUV1 = uv2 - uv1;
            deltaUV2 = uv3 - uv1;
            f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
            bitangent = glm::normalize(bitangent);
            Box->tangent.push_back(tangent);
            Box->tangent.push_back(tangent);
            Box->tangent.push_back(tangent);
            Box->bitangent.push_back(bitangent);
            Box->bitangent.push_back(bitangent);
            Box->bitangent.push_back(bitangent);



            BuildingsVector[i].Mesh[1].push_back(new object_3d(MakeCylinder(vertex1, 0.009, 0.009, 3)));
            BuildingsVector[i].Color[1].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[1].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[1].push_back(0);
            BuildingsVector[i].ColorModeSet[1].push_back(0);
            BuildingsVector[i].Mesh[1].push_back(new object_3d(MakeCylinder(vertex2, vertex3, 0.009, 0.009, 3)));
            BuildingsVector[i].Color[1].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[1].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[1].push_back(0);
            BuildingsVector[i].ColorModeSet[1].push_back(0);

            if (max_valuex < BuildingsVector[i].Points[j].x) {
                max_valuex = BuildingsVector[i].Points[j].x;
                N = j;
            }
            if (_CMATH_::abs(BuildingsVector[i].Points[j].z - BuildingsVector[i].Points[0].z) > 0.1) {
                isRoof = true;
            }
        };
        if (true) {
            BuildingsVector[i].Mesh[3].push_back(new object_3d(MakeBoundBox(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1], BuildingsVector[i].Points[0], 0.4, 0.3, BuildingsVector[i])));
            BuildingsVector[i].Color[3].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[3].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[3].push_back(0);
            BuildingsVector[i].ColorModeSet[3].push_back(0);
            vertex1 = { BuildingsVector[i].Points[0].x , BuildingsVector[i].Points[0].y , 0.299 };
            BuildingsVector[i].Mesh[3].push_back(new object_3d(MakeCylinder(vertex1, 0.4, 0.4, 32)));
            BuildingsVector[i].Color[3].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[3].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[3].push_back(0);
            BuildingsVector[i].ColorModeSet[3].push_back(0);
            vertex1 = { BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].x , BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].y , 0.3 };
            vertex1.z = 0.299;
            BuildingsVector[i].Mesh[3].push_back(new object_3d(MakeCylinder(vertex1, 0.4, 0.4, 32)));
            BuildingsVector[i].Color[3].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[3].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[3].push_back(0);
            BuildingsVector[i].ColorModeSet[3].push_back(0);
            vertex1 = { BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].x , BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].y , 0 };
            vertex2 = { BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].x , BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].y , BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].z };
            vertex3 = { BuildingsVector[i].Points[0].x , BuildingsVector[i].Points[0].y , BuildingsVector[i].Points[0].z };
            Box->vertices.push_back(vertex1);
            Box->vertices.push_back(vertex2);
            Box->vertices.push_back(vertex3);
            norm = GetNormal(vertex1, vertex2, vertex3);

            Box->normals.push_back(norm);
            Box->normals.push_back(norm);
            Box->normals.push_back(norm);

            d2 = distance(vec3(vertex2.x, vertex2.y, 0), vec3(vertex3.x, vertex3.y, 0));

            uv1 = glm::vec2{ 0,0 };
            uv2 = glm::vec2{ 0, BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].z * scale };
            uv3 = glm::vec2{ d2 * scale, BuildingsVector[i].Points[0].z * scale };
            Box->uv.push_back(uv1);
            Box->uv.push_back(uv2);
            Box->uv.push_back(uv3);
            edge1 = vertex2 - vertex1;
            edge2 = vertex3 - vertex1;
            deltaUV1 = uv2 - uv1;
            deltaUV2 = uv3 - uv1;
            f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
            bitangent = glm::normalize(bitangent);
            Box->tangent.push_back(tangent);
            Box->tangent.push_back(tangent);
            Box->tangent.push_back(tangent);
            Box->bitangent.push_back(bitangent);
            Box->bitangent.push_back(bitangent);
            Box->bitangent.push_back(bitangent);


            BuildingsVector[i].Mesh[1].push_back(new object_3d(MakeCylinder(vertex1, 0.009, 0.009, 3)));
            BuildingsVector[i].Color[1].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[1].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[1].push_back(0);
            BuildingsVector[i].ColorModeSet[1].push_back(0);
            BuildingsVector[i].Mesh[1].push_back(new object_3d(MakeCylinder(vertex2, vertex3, 0.009, 0.009, 3)));
            BuildingsVector[i].Color[1].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[1].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[1].push_back(0);
            BuildingsVector[i].ColorModeSet[1].push_back(0);

            vertex1 = { BuildingsVector[i].Points[0].x , BuildingsVector[i].Points[0].y , BuildingsVector[i].Points[0].z };
            vertex2 = { BuildingsVector[i].Points[0].x , BuildingsVector[i].Points[0].y , 0 };
            vertex3 = { BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].x , BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1].y , 0 };
            BuildingsVector[i].Mesh[1].push_back(new object_3d(MakeCylinder(vertex1, vertex2, 0.009, 0.009, 3)));
            BuildingsVector[i].Color[1].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[1].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[1].push_back(0);
            BuildingsVector[i].ColorModeSet[1].push_back(0);

            Box->vertices.push_back(vertex1);
            Box->vertices.push_back(vertex2);
            Box->vertices.push_back(vertex3);
            //NBuilding->colors_backup.push_back(Color2);
            //NBuilding->colors_backup.push_back(Color2);
            //NBuilding->colors_backup.push_back(Color2);
            //NBuilding->colors_alt.push_back(Color2);
            //NBuilding->colors_alt.push_back(Color2);
            //NBuilding->colors_alt.push_back(Color2);
            norm = GetNormal(vertex1, vertex2, vertex3);

            Box->normals.push_back(norm);
            Box->normals.push_back(norm);
            Box->normals.push_back(norm);

            d2 = distance(vec3(vertex2.x, vertex2.y, 0), vec3(vertex3.x, vertex3.y, 0));

            uv1 = glm::vec2{ d2 * scale,BuildingsVector[i].Points[0].z * scale };
            uv2 = glm::vec2{ d2 * scale,0 };
            uv3 = glm::vec2{ 0,0 };

            Box->uv.push_back(uv1);
            Box->uv.push_back(uv2);
            Box->uv.push_back(uv3);
            edge1 = vertex2 - vertex1;
            edge2 = vertex3 - vertex1;
            deltaUV1 = uv2 - uv1;
            deltaUV2 = uv3 - uv1;
            f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
            bitangent = glm::normalize(bitangent);
            Box->tangent.push_back(tangent);
            Box->tangent.push_back(tangent);
            Box->tangent.push_back(tangent);
            Box->bitangent.push_back(bitangent);
            Box->bitangent.push_back(bitangent);
            Box->bitangent.push_back(bitangent);

        };
        BuildingsVector[i].Mesh[0].push_back(Box);
        BuildingsVector[i].Color[0].push_back(glm::vec3(0, 0, 0));
        BuildingsVector[i].ColorSet[0].push_back(glm::vec3(0, 0, 0));
        BuildingsVector[i].ColorMode[0].push_back(0);
        BuildingsVector[i].ColorModeSet[0].push_back(0);
        /// </Box>
        Windows = new object_3dtest(GetWindows2(i, BuildingsVector[i].z, BuildingsVector[i].w));
        if (Windows->size_verticesLI > 0) {
            BuildingsVector[i].Windows.push_back(Windows);
        }
        /// <Roof>
        if (N == 0) {
            N = 1;
        }
        glm::vec3 vert1, vert2, vert3;
        if (N < BuildingsVector[i].Points.size() - 1) {

            vert1.x = BuildingsVector[i].Points[N - 1].x;
            vert1.y = 0;
            vert1.z = BuildingsVector[i].Points[N - 1].y;

            vert2.x = BuildingsVector[i].Points[N].x;
            vert2.y = 0;
            vert2.z = BuildingsVector[i].Points[N].y;

            vert3.x = BuildingsVector[i].Points[N + 1].x;
            vert3.y = 0;
            vert3.z = BuildingsVector[i].Points[N + 1].y;
        }
        else
        {
            N--;
            vert1.x = BuildingsVector[i].Points[N + 1].x;
            vert1.y = 0;
            vert1.z = BuildingsVector[i].Points[N + 1].y;

            vert2.x = BuildingsVector[i].Points[N].x;
            vert2.y = 0;
            vert2.z = BuildingsVector[i].Points[N].y;

            vert3.x = BuildingsVector[i].Points[N - 1].x;
            vert3.y = 0;
            vert3.z = BuildingsVector[i].Points[N - 1].y;
        }
        norm_a = GetNormal(vert1, vert2, vert3);

        vector <glm::vec3> poly;
        vector <glm::vec3> basepoly;

        if (isRoof) {
            int k = 0;
            poly.reserve(BuildingsVector[i].Points.size());
            for (int j = BuildingsVector[i].Points.size() - 1; j > -1; j--) {
                poly.push_back(BuildingsVector[i].Points[j]);
                //basepoly.push_back(base[j]);
            }


            //poly.push_back(BuildingsVector[i].Points[BuildingsVector[i].Points.size() - 1]);
            while ((poly.size() > 3) && ((k + 2) < poly.size())) // if only 3 points are left, we have the final triangle
            {
                int midvertex = FindEar(poly, k); // find the middle vertex of the next "ear"
                /*if ((Det(poly[midvertex - 1].x, poly[midvertex - 1].y, poly[midvertex].x, poly[midvertex].y, poly[midvertex + 1].x, poly[midvertex + 1].y) >= 0)
                     &&(poly.size() <=4)||((poly[midvertex].z < poly[midvertex - 1].z) || (poly[midvertex].z < poly[midvertex + 1].z)))*/
                if ((Det(poly[midvertex - 1].x, poly[midvertex - 1].y, poly[midvertex].x, poly[midvertex].y, poly[midvertex + 1].x, poly[midvertex + 1].y) >= 0) && minVertex(poly, midvertex))
                {
                    Roof->vertices.push_back(poly[midvertex - 1]);
                    Roof->vertices.push_back(poly[midvertex]);
                    Roof->vertices.push_back(poly[midvertex + 1]);
                    //NBuilding->colors_backup.push_back(Color2);
                    //NBuilding->colors_backup.push_back(Color2);
                    //NBuilding->colors_backup.push_back(Color2);
                    //NBuilding->colors_alt.push_back(Color2);
                    //NBuilding->colors_alt.push_back(Color2);
                    //NBuilding->colors_alt.push_back(Color2);
                    norm = GetNormal(poly[midvertex - 1], poly[midvertex], poly[midvertex + 1]);

                    Roof->normals.push_back(norm);
                    Roof->normals.push_back(norm);
                    Roof->normals.push_back(norm);
                    Roof->uv.push_back(glm::vec2{ 0,0 });
                    Roof->uv.push_back(glm::vec2{ 0,0 });
                    Roof->uv.push_back(glm::vec2{ 0,0 });

                    poly.erase(poly.begin() + midvertex);

                    k = 0;
                }
                else
                {
                    k++;
                }
            };
            // only a single triangle remains, so add it to the triangle list
            Roof->vertices.push_back(poly[0]);
            Roof->vertices.push_back(poly[1]);
            Roof->vertices.push_back(poly[2]);
            //NBuilding->colors_backup.push_back(Color2);
            //NBuilding->colors_backup.push_back(Color2);
            //NBuilding->colors_backup.push_back(Color2);
            //NBuilding->colors_alt.push_back(Color2);
            //NBuilding->colors_alt.push_back(Color2);
            //NBuilding->colors_alt.push_back(Color2);
            norm = GetNormal(poly[0], poly[1], poly[2]);

            Roof->normals.push_back(norm);
            Roof->normals.push_back(norm);
            Roof->normals.push_back(norm);
            Roof->uv.push_back(glm::vec2{ 0,0 });
            Roof->uv.push_back(glm::vec2{ 0,0 });
            Roof->uv.push_back(glm::vec2{ 0,0 });

            BuildingsVector[i].Mesh[2].push_back(Roof);
            BuildingsVector[i].Color[2].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[2].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[2].push_back(0);
            BuildingsVector[i].ColorModeSet[2].push_back(0);
        }
        else {
            int k = 0;
            if (norm_a.y > 0) {
                for (int j = BuildingsVector[i].Points.size() - 1; j > -1; j--) {
                    poly.push_back(BuildingsVector[i].Points[j]);
                    //basepoly.push_back(base[j]);
                }
            }
            else {
                for (int j = 0; j < BuildingsVector[i].Points.size(); j++) {
                    poly.push_back(BuildingsVector[i].Points[j]);
                    //basepoly.push_back(base[j]);
                }
            }
            while ((poly.size() > 3)) // if only 3 points are left, we have the final triangle
            {
                int midvertex = FindEar(poly, k); // find the middle vertex of the next "ear"


                Roof->vertices.push_back(poly[midvertex - 1]);
                Roof->vertices.push_back(poly[midvertex]);
                Roof->vertices.push_back(poly[midvertex + 1]);
                norm = GetNormal(poly[midvertex - 1], poly[midvertex], poly[midvertex + 1]);

                Roof->normals.push_back(norm);
                Roof->normals.push_back(norm);
                Roof->normals.push_back(norm);
                Roof->uv.push_back(glm::vec2{ 0,0 });
                Roof->uv.push_back(glm::vec2{ 0,0 });
                Roof->uv.push_back(glm::vec2{ 0,0 });

                poly.erase(poly.begin() + midvertex);

            }
            // only a single triangle remains, so add it to the triangle list
            Roof->vertices.push_back(poly[0]);
            Roof->vertices.push_back(poly[1]);
            Roof->vertices.push_back(poly[2]);
            //NBuilding->colors_backup.push_back(Color2);
            //NBuilding->colors_backup.push_back(Color2);
            //NBuilding->colors_backup.push_back(Color2);
            //NBuilding->colors_alt.push_back(Color2);
            //NBuilding->colors_alt.push_back(Color2);
            //NBuilding->colors_alt.push_back(Color2);
            norm = GetNormal(poly[0], poly[1], poly[2]);

            Roof->normals.push_back(norm);
            Roof->normals.push_back(norm);
            Roof->normals.push_back(norm);
            Roof->uv.push_back(glm::vec2{ 0,0 });
            Roof->uv.push_back(glm::vec2{ 0,0 });
            Roof->uv.push_back(glm::vec2{ 0,0 });
            BuildingsVector[i].Mesh[2].push_back(Roof);
            BuildingsVector[i].Color[2].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorSet[2].push_back(glm::vec3(0, 0, 0));
            BuildingsVector[i].ColorMode[2].push_back(0);
            BuildingsVector[i].ColorModeSet[2].push_back(0);
        }
    }

}

void MakeRods(int start, int end) {
    glm::vec3 vertex1, vertex2, vertex3;
    glm::vec3 norm, norm_a;

    for (int i = start; i < end; i++) {
        RodsVector[i].SetIndexAlt(UniqueID());
        Table.push_back(Object{ RodsVector[i].UnIndex ,(unsigned long)i ,RODS });
        object_3d* RodObj, * BorderObj;
        RodsVector[i].Mesh[0].reserve(RodsVector[i].Points.size());
        RodsVector[i].Mesh[1].reserve(RodsVector[i].Points.size());
        RodsVector[i].Color[0].reserve(RodsVector[i].Points.size());
        RodsVector[i].Color[1].reserve(RodsVector[i].Points.size());
        RodsVector[i].ColorSet[0].reserve(RodsVector[i].Points.size());
        RodsVector[i].ColorSet[1].reserve(RodsVector[i].Points.size());
        RodsVector[i].ColorMode[0].reserve(RodsVector[i].Points.size());
        RodsVector[i].ColorMode[1].reserve(RodsVector[i].Points.size());
        RodsVector[i].ColorModeSet[0].reserve(RodsVector[i].Points.size());
        RodsVector[i].ColorModeSet[1].reserve(RodsVector[i].Points.size());
        for (int j = 0; j < RodsVector[i].Points.size(); j++)
        {
            RodObj = new object_3d(MakeRod(RodsVector[i].Points[j]));
            if (RodObj->vertices.size() > 0) {
                RodsVector[i].Mesh[0].push_back(RodObj);
                RodsVector[i].Color[0].push_back(glm::vec3(0, 0, 0));
                RodsVector[i].ColorSet[0].push_back(glm::vec3(0, 0, 0));
                RodsVector[i].ColorMode[0].push_back(0);
                RodsVector[i].ColorModeSet[0].push_back(0);
            }
            else {
                delete RodObj;
            };
            BorderObj = new object_3d(MakeBorderRod(RodsVector[i].Points[j]));
            if (BorderObj->vertices.size() > 0) {
                RodsVector[i].Mesh[1].push_back(BorderObj);
                RodsVector[i].Color[1].push_back(glm::vec3(0, 0, 0));
                RodsVector[i].ColorSet[1].push_back(glm::vec3(0, 0, 0));
                RodsVector[i].ColorMode[1].push_back(0);
                RodsVector[i].ColorModeSet[1].push_back(0);
            }
            else {
                delete BorderObj;
            }
        };
        //RodsVector[i].GetCenter();
    }

}

void MakeCRods(int start, int end) {
    glm::vec3 vertex1, vertex2, vertex3;
    glm::vec3 norm, norm_a;

    for (int i = start; i < end; i++) {
        CRodsVector[i].SetIndexAlt(UniqueID());
        Table.push_back(Object{ CRodsVector[i].UnIndex ,(unsigned long)i ,CRODS });
        TVector4f p1, p2;
        CRodsVector[i].Mesh[0].reserve(CRodsVector[i].Points.size());
        CRodsVector[i].Mesh[1].reserve(CRodsVector[i].Points.size());
        CRodsVector[i].Mesh[2].reserve(CRodsVector[i].Points.size() - 1);
        CRodsVector[i].Color[0].reserve(CRodsVector[i].Points.size());
        CRodsVector[i].Color[1].reserve(CRodsVector[i].Points.size());
        CRodsVector[i].Color[2].reserve(CRodsVector[i].Points.size() - 1);
        CRodsVector[i].ColorSet[0].reserve(CRodsVector[i].Points.size());
        CRodsVector[i].ColorSet[1].reserve(CRodsVector[i].Points.size());
        CRodsVector[i].ColorSet[2].reserve(CRodsVector[i].Points.size() - 1);
        CRodsVector[i].ColorMode[0].reserve(CRodsVector[i].Points.size());
        CRodsVector[i].ColorMode[1].reserve(CRodsVector[i].Points.size());
        CRodsVector[i].ColorMode[2].reserve(CRodsVector[i].Points.size() - 1);
        CRodsVector[i].ColorModeSet[0].reserve(CRodsVector[i].Points.size());
        CRodsVector[i].ColorModeSet[1].reserve(CRodsVector[i].Points.size());
        CRodsVector[i].ColorModeSet[2].reserve(CRodsVector[i].Points.size() - 1);
        for (int j = 0; j < CRodsVector[i].Points.size(); j++)
        {
            object_3d* CRodObj;
            CRodObj = new object_3d(MakeRod(CRodsVector[i].Points[j]));
            if (CRodObj->vertices.size() > 0) {
                CRodsVector[i].Mesh[0].push_back(CRodObj);
                CRodsVector[i].Color[0].push_back(glm::vec3(0, 0, 0));
                CRodsVector[i].ColorSet[0].push_back(glm::vec3(0, 0, 0));
                CRodsVector[i].ColorMode[0].push_back(0);
                CRodsVector[i].ColorModeSet[0].push_back(0);
                object_3d* Border = new object_3d(MakeBorderRod(CRodsVector[i].Points[j]));
                CRodsVector[i].Mesh[1].push_back(Border);
                CRodsVector[i].Color[1].push_back(glm::vec3(0, 0, 0));
                CRodsVector[i].ColorSet[1].push_back(glm::vec3(0, 0, 0));
                CRodsVector[i].ColorMode[1].push_back(0);
                CRodsVector[i].ColorModeSet[1].push_back(0);
            }
            else {
                delete CRodObj;
            }
            if (j > 0) {
                object_3d* Cable = new object_3d(MakeCylinder(CRodsVector[i].Points[j - 1], CRodsVector[i].Points[j], 0.02, 0.02, 8));
                if (Cable->vertices.size() > 0) {
                    CRodsVector[i].Mesh[2].push_back(Cable);
                    CRodsVector[i].Color[2].push_back(glm::vec3(0, 0, 0));
                    CRodsVector[i].ColorSet[2].push_back(glm::vec3(0, 0, 0));
                    CRodsVector[i].ColorMode[2].push_back(0);
                    CRodsVector[i].ColorModeSet[2].push_back(0);
                    //object_3d* Border = new object_3d(MakeCylinderBorder(CRodsVector[i].Points[j - 1], CRodsVector[i].Points[j], 0.009, 0.009, 8));
                    //CRodsVector[i].Mesh[1].push_back(Border);
                }
                else {
                    delete Cable;
                }
            }
        }
    }

}

void MakePL(int start, int end) {

    for (int i = start; i < end; i++) {
        PLVector[i].SetIndexAlt(UniqueID());
        Table.push_back(Object{ PLVector[i].UnIndex ,(unsigned long)i ,POWERLINE });
        PLVector[i].Mesh[0].reserve(PLVector[i].Points.size());
        PLVector[i].Mesh[1].reserve(PLVector[i].Points.size() + PLVector[i].Points.size() - 1);
        PLVector[i].Mesh[2].reserve(PLVector[i].Points.size() - 1);
        PLVector[i].Color[0].reserve(PLVector[i].Points.size());
        PLVector[i].Color[1].reserve(PLVector[i].Points.size() + PLVector[i].Points.size() - 1);
        PLVector[i].Color[2].reserve(PLVector[i].Points.size() - 1);
        PLVector[i].ColorSet[0].reserve(PLVector[i].Points.size());
        PLVector[i].ColorSet[1].reserve(PLVector[i].Points.size() + PLVector[i].Points.size() - 1);
        PLVector[i].ColorSet[2].reserve(PLVector[i].Points.size() - 1);
        PLVector[i].ColorMode[0].reserve(PLVector[i].Points.size());
        PLVector[i].ColorMode[1].reserve(PLVector[i].Points.size() + PLVector[i].Points.size() - 1);
        PLVector[i].ColorMode[2].reserve(PLVector[i].Points.size() - 1);
        PLVector[i].ColorModeSet[0].reserve(PLVector[i].Points.size());
        PLVector[i].ColorModeSet[1].reserve(PLVector[i].Points.size() + PLVector[i].Points.size() - 1);
        PLVector[i].ColorModeSet[2].reserve(PLVector[i].Points.size() - 1);
        for (int j = 0; j < PLVector[i].Points.size(); j++)
        {
            object_3d* PLObj = new object_3d(MakeCylinder(PLVector[i].Points[j], 0.3, 0.3, 8));
            if (PLObj->vertices.size() > 0) {
                PLVector[i].Mesh[0].push_back(PLObj);
                PLVector[i].Color[0].push_back(glm::vec3(0, 0, 0));
                PLVector[i].ColorSet[0].push_back(glm::vec3(0, 0, 0));
                PLVector[i].ColorMode[0].push_back(0);
                PLVector[i].ColorModeSet[0].push_back(0);
                PLVector[i].Mesh[1].push_back(new object_3d(MakeCylinderBorder(PLVector[i].Points[j], 0.3, 0.3, 8)));
                PLVector[i].Color[1].push_back(glm::vec3(0, 0, 0));
                PLVector[i].ColorSet[1].push_back(glm::vec3(0, 0, 0));
                PLVector[i].ColorMode[1].push_back(0);
                PLVector[i].ColorModeSet[1].push_back(0);
            }
            else {
                delete PLObj;
            }
            if (j > 0) {
                object_3d* Cable = new object_3d(MakeCylinder(PLVector[i].Points[j - 1], PLVector[i].Points[j], 0.1, 0.1, 8));
                if (Cable->vertices.size() > 0) {
                    PLVector[i].Mesh[2].push_back(Cable);
                    PLVector[i].Color[2].push_back(glm::vec3(0, 0, 0));
                    PLVector[i].ColorSet[2].push_back(glm::vec3(0, 0, 0));
                    PLVector[i].ColorMode[2].push_back(0);
                    PLVector[i].ColorModeSet[2].push_back(0);
                    PLVector[i].Mesh[1].push_back(new object_3d(MakeCylinderBorder(PLVector[i].Points[j - 1], PLVector[i].Points[j], 0.1, 0.1, 8)));
                    PLVector[i].Color[1].push_back(glm::vec3(0, 0, 0));
                    PLVector[i].ColorSet[1].push_back(glm::vec3(0, 0, 0));
                    PLVector[i].ColorMode[1].push_back(0);
                    PLVector[i].ColorModeSet[1].push_back(0);
                }
                else {
                    delete Cable;
                }
            }
        }
    }

};

void MakeZones(int count) {
    int stepRow = _CMATH_::ceil(MP.GetRow() / count);
    int stepCol = _CMATH_::ceil(MP.GetCol() / count);
    /*
    for (int i = 1; i <= count; i++)
    {
        for (int j = 1; j <= count; j++) {
            if ((i < count) && (j < count)) {
                MakeTrianglesDynamic((i - 1) * stepRow, (i)*stepRow, (j - 1) * stepCol, (j)*stepCol);
            }
            else if ((i < count) && (j = count)) {
                MakeTrianglesDynamic((i - 1) * stepRow, (i)*stepRow, (j - 1) * stepCol, MP.GetCol() - 1);
            }
            else if ((i = count) && (j < count))
            {
                MakeTrianglesDynamic((i - 1) * stepRow, MP.GetRow() - 1, (j - 1) * stepCol, (j)*stepCol);
            }
            else {

                MakeTrianglesDynamic((i - 1) * stepRow, MP.GetRow() - 1, (j - 1) * stepCol, MP.GetCol() - 1);
            }
        }
    }
    */
    Mesh_type ZoneM;
    ZoneM.n = 1;
    genVectors(ZoneM);
    object_3d* Zone;
    //object_3d* Zone = new object_3d (MakeTrianglesDynamic(0, MP.GetRow() - 1, 0, MP.GetCol() - 1));
    if (M->IndexVBO) {
        Zone = new object_3d(MakeTrianglesDynamic2(0, MP.GetRow(), 0, MP.GetCol()));
    }
    else {
        Zone = new object_3d(MakeTrianglesDynamic(0, MP.GetRow() - 1, 0, MP.GetCol() - 1));
    }
    if (Zone->vertices.size() > 0) {
        ZoneM.SetIndexAlt(UniqueID());
        Table.push_back(Object{ ZoneM.UnIndex ,(unsigned long)0 ,ZONE });
        ZoneM.Mesh[0].push_back(Zone);
        ZoneM.Color[0].push_back(glm::vec3(0, 0, 0));
    }



    ZoneVector.push_back(ZoneM);



    float Size = 1;
    glm::vec3 vertex;
    glm::vec3 norm;
    int width = 15;
    min_x = MP.GetItem(0, 0).x - width;
    max_X = MP.GetItem(MP.GetRow() - 1, MP.GetCol() - 1).x + width;
    max_Y = MP.GetItem(0, 0).y - width;
    min_Y = MP.GetItem(MP.GetRow() - 1, MP.GetCol() - 1).y + width;
    bmin_x = MP.GetItem(0, 0).x;
    bmax_X = MP.GetItem(MP.GetRow() - 1, MP.GetCol() - 1).x;
    bmax_Y = MP.GetItem(0, 0).y;
    bmin_Y = MP.GetItem(MP.GetRow() - 1, MP.GetCol() - 1).y;
    MP.Destroy();


    std::vector<thread> th_vec;

    MakeRods(0, RodsVector.size());
    MakeCRods(0, CRodsVector.size());
    MakePL(0, PLVector.size());
    MakeBuildings(0, BuildingsVector.size());
    MakeEBuildings(0, EBuildingsMeshVector.size());
    MakeBridges(0, BridgeVector.size());
    MakeTubes(0, TubeMeshVector.size());
    /*
    for (int i = 1; i <= RodsVector.size(); i++) {
        th_vec.push_back(thread(MakeRods, i - 1, i));
    }
    for (int i = 1; i <= CRodsVector.size(); i++) {
        th_vec.push_back(thread(MakeCRods, i - 1, i));
    }
    for (int i = 1; i <= BuildingsVector.size(); i++) {
        th_vec.push_back(thread(MakeBuildings, i - 1, i));
    }
    for (int i = 1; i <= PLVector.size(); i++) {
        th_vec.push_back(thread(MakePL, i-1, i));
    }
    int Thcount = th_vec.size();
    for (int i = 0; i < Thcount; i++)
    {
        th_vec.at(i).join();
    };
    th_vec.clear();*/

    /*for (int i = 0; i < Borders.size(); i++) {
        TVector4f p1, p2, p3;
        p1.x = Borders[i].x1;
        p1.y = Borders[i].y1;
        p1.z = Borders[i].z1;
        p2.x = Borders[i].x2;
        p2.y = Borders[i].y2;
        p2.z = Borders[i].z2;
        Cylinder C = drawCylinder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 0.05, 0.05, false, 8);
        for (int i = 0; i < C.vertices.size(); i++)
        {
            Border.vertices.push_back(C.vertices[i]);
            //colors.push_back(C.colors[i]);
            Border.colors_backup.push_back(Color4); Border.colors_alt.push_back(Color4);
            Border.normals.push_back(C.normals[i]);
        }
    }
    for (int i = 0; i < Borders2.size(); i++) {
        TVector4f p1, p2, p3;
        p1.x = Borders2[i].x1;
        p1.y = Borders2[i].y1;
        p1.z = Borders2[i].z1;
        p2.x = Borders2[i].x2;
        p2.y = Borders2[i].y2;
        p2.z = Borders2[i].z2;
        Cylinder C = drawCylinder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, 0.009, 0.009, false, 8);
        for (int i = 0; i < C.vertices.size(); i++)
        {
            Border.vertices.push_back(C.vertices[i]);
            //colors.push_back(C.colors[i]);
            Border.colors_backup.push_back(Color4); Border.colors_alt.push_back(Color4);
            Border.normals.push_back(C.normals[i]);
        }
    }*/

    /*Cylinder C = drawCylinder(lightPos.x, lightPos.y, 0, lightPos.x, lightPos.y, lightPos.z, 0.5, 0.5, false, 4);
    for (int i = 0; i < C.vertices.size(); i++)
    {
        Border.vertices.push_back(C.vertices[i]);
        //colors.push_back(C.colors[i]);
        Border.colors_backup.push_back(Color4); Border.colors_alt.push_back(Color4);
        Border.normals.push_back(C.normals[i]);
    }*/

    //Border.SetIndexAlt(UniqueID());
    //Table.push_back(Object{ Border.UnIndex ,0 ,SomeObj });
}

template < typename T>
void SetValue(T& value, vector<string>& arr) {
    int size = sizeof(T) / sizeof(float) + 1;
    if (arr.size() == size) {
        cout << arr[0];
        for (int i = 1; i < size; i++) {
            value[i - 1] = stof(arr[i]) / 255.0f;
            cout << " " << value[i - 1];
        }
        cout << endl;
    }
}

void InCommand(string str)
{
    string delim(" ");
    size_t prev = 0;
    vector<string> arr;
    size_t next;
    size_t delta = delim.length();
    while ((next = str.find(delim, prev)) != string::npos) {
        arr.push_back(str.substr(prev, next - prev));
        prev = next + delta;
    }
    if (prev < str.size()) {
        arr.push_back(str.substr(prev));
    }
    if (arr.size() > 0) {
        if (arr[0] == "IndexVBO") {
            if (stoi(arr[1]) > 0) {
                M->IndexVBO = true;
            }
            else {
                M->IndexVBO = false;
            }
            cout << "IndexVBO" << " " << M->IndexVBO << endl;
        }
        else if (arr[0] == "Shadows") {
            if (stoi(arr[1]) > 0) {
                M->EnableShadows = true;
            }
            else {
                M->EnableShadows = false;
            }
            cout << "Shadows" << " " << M->EnableShadows << endl;
        }
        else if (arr[0] == "ColorZone") {
            SetValue(M->ColorZone, arr);
        }
        else if (arr[0] == "ColorRods") {
            SetValue(M->ColorRods, arr);
        }
        else if (arr[0] == "ColorRodsBorder") {
            SetValue(M->ColorRodsBorder, arr);
        }
        else if (arr[0] == "ColorCRods") {
            SetValue(M->ColorCRods, arr);
        }
        else if (arr[0] == "ColorCRodsBorder") {
            SetValue(M->ColorCRodsBorder, arr);
        }
        else if (arr[0] == "ColorCRodsCable") {
            SetValue(M->ColorCRodsCable, arr);
        }
        else if (arr[0] == "ColorPL") {
            SetValue(M->ColorPL, arr);
        }
        else if (arr[0] == "ColorPLBorder") {
            SetValue(M->ColorPLBorder, arr);
        }
        else if (arr[0] == "ColorPLCable") {
            SetValue(M->ColorPLCable, arr);
        }
        else if (arr[0] == "ColorBuildingBox") {
            SetValue(M->ColorBuildingBox, arr);
        }
        else if (arr[0] == "ColorBuildingFloor") {
            SetValue(M->ColorBuildingFloor, arr);
        }
        else if (arr[0] == "ColorBuildingRoof") {
            SetValue(M->ColorBuildingRoof, arr);
        }
        else if (arr[0] == "ColorBuildingBorder") {
            SetValue(M->ColorBuildingBorder, arr);
        }
        else if (arr[0] == "ColorSky") {
            SetValue(M->ColorSky, arr);
        }
        else if (arr[0] == "BuildingTexture") {
            if (arr.size() == 2) {
                M->item_Building = stoi(arr[1]);
            }
            cout << "BuildingTexture" << " " << M->item_Building << endl;
        }
        else if (arr[0] == "UseColorBuilding") {
            if (stoi(arr[1]) > 0) {
                M->UseColor_Building = true;
            }
            else {
                M->UseColor_Building = false;
            }
            cout << "UseColorBuilding" << " " << M->UseColor_Building << endl;
        }
        else if (arr[0] == "ShadowScale") {
            if (arr.size() == 2) {
                M->ShadowScale = stoi(arr[1]);
                M->ShadowScaleLast = M->ShadowScale;
            }
            cout << "ShadowScale" << " " << M->ShadowScale << endl;
        }
        else if (arr[0] == "lightPower") {
            if (arr.size() == 2) {
                M->lightPower = stof(arr[1]);
            }
            cout << "lightPower" << " " << M->lightPower << endl;
        }
        else if (arr[0] == "OrthographicShadow") {
            if (arr.size() == 2) {
                if (stoi(arr[1]) > 0)
                {
                    M->OrthographicShadow = true;
                }
                else {
                    M->OrthographicShadow = false;
                }
            }
            cout << "OrthographicShadow" << " " << M->OrthographicShadow << endl;
        }
        else if (arr[0] == "OrthographicTerra") {
            if (arr.size() == 2) {
                if (stoi(arr[1]) > 0)
                {
                    M->OrthographicTerra = true;
                    M->OrthographicTerraF = 1.0f;
                }
                else {
                    M->OrthographicTerra = false;
                    M->OrthographicTerraF = 0.0f;
                }
            }
            cout << "OrthographicTerra" << " " << M->OrthographicTerra << endl;
        }
        else if (arr[0] == "OrthographicObjects") {
            if (arr.size() == 2) {
                if (stoi(arr[1]) > 0)
                {
                    M->OrthographicObjects = true;
                    M->OrthographicObjectsF = 1.0f;
                }
                else {
                    M->OrthographicObjects = false;
                    M->OrthographicObjectsF = 0.0f;
                }
                cout << "OrthographicObjects" << " " << M->OrthographicObjects << endl;
            }
        }
        else if (arr[0] == "OrthographicBuildings") {
            if (arr.size() == 2) {
                if (stoi(arr[1]) > 0)
                {
                    M->OrthographicBuildings = true;
                    M->OrthographicBuildingsF = 1.0f;
                }
                else {
                    M->OrthographicBuildings = false;
                    M->OrthographicBuildingsF = 0.0f;
                }
                cout << "OrthographicBuildings" << " " << M->OrthographicBuildings << endl;
            }
        }
        else if (arr[0] == "biasOrthographic") {
            if (arr.size() == 2) {
                if (stoi(arr[1]) > 0)
                {
                    M->biasOrthographic = stoi(arr[1]) / 100000000.0f;
                }
                cout << "biasOrthographic" << " " << M->biasOrthographic << endl;
            }
        }
        else if (arr[0] == "biasPerspective") {
            if (arr.size() == 2) {
                if (stoi(arr[1]) > 0)
                {
                    M->biasPerspective = stoi(arr[1]) / 100000000.0f;
                }
                cout << "biasPerspective" << " " << M->biasPerspective << endl;
            }
        }
        else if (arr[0] == "lightPos") {
            if (arr.size() == 4) {
                M->lightPos.x = stof(arr[1]);
                M->lightPos.y = stof(arr[2]);
                M->lightPos.z = stof(arr[3]);
                cout << "lightPos" << " " << M->lightPos.x << " " << M->lightPos.y << " " << M->lightPos.z << endl;
            }
        }
        else if (arr[0] == "LightDirection") {
            if (arr.size() == 4) {
                M->LightDirection.x = stof(arr[1]);
                M->LightDirection.y = stof(arr[2]);
                M->LightDirection.z = stof(arr[3]);
                cout << "LightDirection" << " " << M->LightDirection.x << " " << M->LightDirection.y << " " << M->LightDirection.z << endl;
            }
        }
        else if (arr[0] == "Theme") {
            if (arr.size() == 2) {
                if (arr[1] == "DARK") {
                    Theme = DARK;
                    ThemeIndex = 0;
                }
                else if (arr[1] == "CLASSIC") {
                    Theme = CLASSIC;
                    ThemeIndex = 1;
                }
                else if (arr[1] == "LIGHT") {
                    Theme = LIGHT;
                    ThemeIndex = 2;
                }
            }
        }
        else if (arr[0] == "Mode") {
            if (stoi(arr[1]) < MVector.size())
            {
                M = &MVector[stoi(arr[1])];
            }
            else {
                M = &MVector[0];
            }
        }
        else if (arr[0] == "BindMode") {
            if (arr.size() == 2)
            {
                if (stoi(arr[1]) == 0) {
                    BindMode = false;
                }
                else {
                    BindMode = true;
                }
            }

        }
        else if (arr[0] == "BindedMode") {
            if (arr.size() == 2)
            {
                BindedMode = stoi(arr[1]);
            }
        }
        else if (arr[0] == "BindedModeLast") {
            if (arr.size() == 2)
            {
                BindedModeLast = stoi(arr[1]);
            }
        }
        else if (arr[0] == "currentMode") {
            if (arr.size() == 2)
            {
                currentMode = stoi(arr[1]);
            }
        }
        else if (arr[0] == "currentModeLast") {
            if (arr.size() == 2)
            {
                currentModeLast = stoi(arr[1]);
            }
        }
        else if (arr[0] == "BindKey1") {
            if (arr.size() == 2)
            {
                BindKey1 = stoi(arr[1]);
            }
        }
        else if (arr[0] == "BindKey2") {
            if (arr.size() == 2)
            {
                BindKey2 = stoi(arr[1]);
            }
        }
        else if (arr[0] == "BindKey3") {
            if (arr.size() == 2)
            {
                BindKey3 = stoi(arr[1]);
            }
        }
        else if (arr[0] == "BindKey4") {
            if (arr.size() == 2)
            {
                BindKey4 = stoi(arr[1]);
            }
        }
        else {
        }
    }
}
void InCommandTextures(string str)
{
    string delim(" ");
    size_t prev = 0;
    vector<string> arr;
    size_t next;
    size_t delta = delim.length();
    while ((next = str.find(delim, prev)) != string::npos) {
        arr.push_back(str.substr(prev, next - prev));
        prev = next + delta;
    }
    if (prev < str.size()) {
        arr.push_back(str.substr(prev));
    }
    if (arr.size() == 2) {
        if (arr[0] == "name") {
            TextureObject TexObj;
            TexObj.Name = arr[1];
            Textures.push_back(TexObj);

        }
        else
            if (arr[0] == "diff") {
                //const char* c = arr[1].c_str();
                Textures[Textures.size() - 1].Texture = loadTexture(arr[1].c_str());
            }
            else
                if (arr[0] == "norm") {
                    //const char* c = arr[1].c_str();
                    Textures[Textures.size() - 1].Normal = loadTexture(arr[1].c_str());
                }
                else
                    if (arr[0] == "disp") {
                        //const char* c = arr[1].c_str();
                        Textures[Textures.size() - 1].Disp = loadTexture(arr[1].c_str());
                    }

    }
}

void PrintVec3(std::ofstream& out, glm::vec3& p, string& s) {
    int r, g, b;
    r = round(p.x * 255.0f);
    g = round(p.y * 255.0f);
    b = round(p.z * 255.0f);
    out << s << " " << (int)r << " " << (int)g << " " << (int)b << endl;
}
void SaveMode(std::ofstream& out) {
    string s;
    if (M->IndexVBO) {
        out << "IndexVBO" << " " << 1 << endl;
    }
    else {
        out << "IndexVBO" << " " << 0 << endl;
    }
    if (M->EnableShadows) {
        out << "Shadows" << " " << 1 << endl;
    }
    else {
        out << "Shadows" << " " << 0 << endl;
    }
    s = "ColorZone";
    PrintVec3(out, M->ColorZone, s);
    s = "ColorRods";
    PrintVec3(out, M->ColorRods, s);
    s = "ColorRodsBorder";
    PrintVec3(out, M->ColorRodsBorder, s);
    s = "ColorCRods";
    PrintVec3(out, M->ColorCRods, s);
    s = "ColorCRodsBorder";
    PrintVec3(out, M->ColorCRodsBorder, s);
    s = "ColorCRodsCable";
    PrintVec3(out, M->ColorCRodsCable, s);
    s = "ColorPL";
    PrintVec3(out, M->ColorPL, s);
    s = "ColorPLBorder";
    PrintVec3(out, M->ColorPLBorder, s);
    s = "ColorPLCable";
    PrintVec3(out, M->ColorPLCable, s);
    s = "ColorBuildingBox";
    PrintVec3(out, M->ColorBuildingBox, s);
    s = "ColorBuildingFloor";
    PrintVec3(out, M->ColorBuildingFloor, s);
    s = "ColorBuildingRoof";
    PrintVec3(out, M->ColorBuildingRoof, s);
    s = "ColorBuildingBorder";
    PrintVec3(out, M->ColorBuildingBorder, s);
    s = "ColorSky";
    PrintVec3(out, M->ColorSky, s);
    s = "BuildingTexture";
    out << s << " " << M->item_Building << endl;
    if (M->UseColor_Building) {
        out << "UseColorBuilding" << " " << 1 << endl;
    }
    else {
        out << "UseColorBuilding" << " " << 0 << endl;
    }
    s = "ShadowScale";
    out << s << " " << M->ShadowScale << endl;
    s = "lightPower";
    out << s << " " << (float)M->lightPower << endl;
    if (M->OrthographicShadow) {
        out << "OrthographicShadow" << " " << 1 << endl;
    }
    else {
        out << "OrthographicShadow" << " " << 0 << endl;
    }
    s = "OrthographicTerra";
    out << s << " " << (float)M->OrthographicTerraF << endl;
    s = "OrthographicObjects";
    out << s << " " << (float)M->OrthographicObjectsF << endl;
    s = "OrthographicBuildings";
    out << s << " " << (float)M->OrthographicBuildingsF << endl;
    s = "biasOrthographic";
    out << s << " " << (float)M->biasOrthographic * 100000000 << endl;
    s = "biasPerspective";
    out << s << " " << (float)M->biasPerspective * 100000000 << endl;
    s = "lightPos";
    out << s << " " << M->lightPos.x << " " << M->lightPos.y << " " << M->lightPos.z << endl;
    s = "LightDirection";
    out << s << " " << M->LightDirection.x << " " << M->LightDirection.y << " " << M->LightDirection.z << endl;
}

void SaveConfig(string& path) {
    std::ofstream out;          // поток для записи
    string s;
    out.open(path); // окрываем файл для записи
    if (out.is_open())
    {
        s = "Theme ";
        if (Theme == DARK)
        {
            out << s << "DARK" << endl;
        }
        else if (Theme == CLASSIC) {
            out << s << "CLASSIC" << endl;
        }
        else if (Theme == LIGHT) {
            out << s << "LIGHT" << endl;
        }

        if (BindMode) {
            out << "BindMode" << " " << 1 << endl;
        }
        else {
            out << "BindMode" << " " << 0 << endl;
        }
        s = "BindedMode";
        out << s << " " << BindedMode << endl;
        s = "BindedModeLast";
        out << s << " " << BindedModeLast << endl;
        s = "currentMode";
        out << s << " " << currentMode << endl;
        s = "currentModeLast";
        out << s << " " << currentModeLast << endl;
        s = "BindKey1";
        out << s << " " << BindKey1 << endl;
        s = "BindKey2";
        out << s << " " << BindKey2 << endl;
        s = "BindKey3";
        out << s << " " << BindKey3 << endl;
        s = "BindKey4";
        out << s << " " << BindKey4 << endl;

        out << "Mode 0" << endl;
        M = &MVector[0];
        SaveMode(out);
        out << "Mode 1" << endl;
        M = &MVector[1];
        SaveMode(out);
    }
    out.close();
}
void MakeIO() {
    int Total = 0;
    Total += 1;
    IO.ZoneIndex = Total;
    Total += 1;
    IO.vertexIndex = Total;
    Total += 1;
    IO.BorderIndex = Total;
    Total += 1;
    IO.Rays = Total;
    Total = Total + Rays.size();
    IO.Buildings_triangles = Total;
    Total = Total + BuildingsVector.size();
    IO.terraIndex = Total;
}

glm::mat4 ViewMatrix, ViewMatrix2;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
    return ViewMatrix;
}
glm::mat4 getViewMatrixZY() {
    return ViewMatrix2;
}
glm::mat4 getProjectionMatrix() {
    return ProjectionMatrix;
}

void computeMatricesFromInputs(int winW, int winH) {


    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;

    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, winW / 2, winH / 2);


    // Compute new orientation
    horizontalAngle += mouseSpeed * float(winW / 2 - xpos);
    verticalAngle += mouseSpeed * float(winH / 2 - ypos);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    /*glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );*/
    glm::vec3 direction(
        cos(verticalAngle) * cos(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * sin(horizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }

    float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 1000.0f);
    // Camera matrix
    ViewMatrix = glm::lookAt(
        position,           // Camera is here
        position + direction, // and looks here : at the same position, plus "direction"
        up                  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}

glm::vec3 CreateRay() {
    // these positions must be in range [-1, 1] (!!!), not [0, width] and [0, height]
    // Get mouse position
    double xpos, ypos;
    //glfwGetCursorPos(window, &xpos, &ypos);
    float mouseX = (winW / 2) / (winW * 0.5f) - 1.0f;
    float mouseY = (winH / 2) / (winH * 0.5f) - 1.0f;


    glm::mat4 invVP = glm::inverse(ProjectionMatrix * ViewMatrix);
    glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
    glm::vec4 worldPos = invVP * screenPos;

    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

    return dir;
}


void mouse_clickMakeRay() {

    GLfloat zf;
    GLuint index;
    //GLubyte color[4];
    float color[4];
    long id;


    //glReadPixels((winW / 2), (winH / 2), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zf);
    //glReadPixels((winW / 2), (winH / 2), 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
    glFlush();
    glFinish();
    glReadPixels((winW / 2), (winH / 2), 1, 1, GL_RGBA, GL_FLOAT, color);

    //glm::vec4 viewport = glm::vec4(0, 0, winW, winH);
    //glm::vec3 wincoord = glm::vec3((winW / 2), winH - (winH / 2) - 1, zf);
    //glm::vec3 objcoord = glm::unProject(wincoord, ViewMatrix, ProjectionMatrix, viewport);

    id = rgbToDecimal(color[0], color[1], color[2]);
    cout << "Clicked on pixel color " << id << endl;
    /* if (id < Table.size()) {
         if (Table[id].b == BUILDING) {
             BuildingsVector[Table[id].index].selected = true;
             if (TableSelected.size() > 0) {
                 if (TableSelected[0].indexGlobal != id) {
                     BuildingsVector[Table[TableSelected[0].indexGlobal].index].selected = false;
                     TableSelected.erase(TableSelected.begin());
                     TableSelected.push_back(Table[id]);
                     //m_currentMesh = BuildingsVector[Table[id].index].Wall_texture;
                     item_current = BuildingsVector[Table[id].index].Wall_texture;
                     cz = BuildingsVector[Table[id].index].z;
                     cw = BuildingsVector[Table[id].index].w;
                     SelectColor3 = BuildingsVector[Table[id].index].ColorBuilding;
                     SelectColor = BuildingsVector[Table[id].index].SelectColor;
                 }
                 else {
                     BuildingsVector[Table[TableSelected[0].indexGlobal].index].selected = false;
                     TableSelected.erase(TableSelected.begin());
                 }
             }
             else {
                 TableSelected.push_back(Table[id]);
                 //m_currentMesh = BuildingsVector[Table[id].index].Wall_texture;
                 item_current = BuildingsVector[Table[id].index].Wall_texture;
                 cz = BuildingsVector[Table[id].index].z;
                 cw = BuildingsVector[Table[id].index].w;
                 SelectColor3 = BuildingsVector[Table[id].index].ColorBuilding;
                 SelectColor = BuildingsVector[Table[id].index].SelectColor;
             }
             Inspector.SetBuildingsInspector();
         }
         else if (Table[id].b == TERRA)
         {
             Inspector.SetTerraInspector();
         }
         else if (Table[id].b == RODS)
         {
             RodsVector[Table[id].index].selected = true;
             if (TableSelected.size() > 0) {
                 if (TableSelected[0].indexGlobal != id) {
                     RodsVector[Table[TableSelected[0].indexGlobal].index].selected = false;
                     TableSelected.erase(TableSelected.begin());
                     TableSelected.push_back(Table[id]);
                     SelectColor3 = RodsVector[Table[id].index].ColorRod;
                 }
                 else {
                     RodsVector[Table[TableSelected[0].indexGlobal].index].selected = false;
                     TableSelected.erase(TableSelected.begin());
                 }
             }
             else {
                 TableSelected.push_back(Table[id]);
                 SelectColor3 = RodsVector[Table[id].index].ColorRod;
             }
             Inspector.SetRodsInspector();
         }
         else {
             Inspector.False();
         }
     }
     else {
         Inspector.SetSkyInspector();
     }
     */
    if (id < Table.size()) {
        if (TableSelected.size() > 0) {
            if (TableSelected[0].indexGlobal != id) {
                Table[TableSelected[0].indexGlobal].selected = false;
                //Table[TableSelected[0].indexGlobal].returnVertices(RodsBuff,RodsVector[TableSelected[0].index]);
                TableSelected.erase(TableSelected.begin());
                Table[id].selected = true;
                TableSelected.push_back(Table[id]);
            }
            else {

                Table[TableSelected[0].indexGlobal].selected = false;
                //Table[TableSelected[0].indexGlobal].returnVertices(RodsBuff, RodsVector[TableSelected[0].index]);
                TableSelected.erase(TableSelected.begin());
                Inspector.False();
            }
        }
        else {
            Table[id].selected = true;
            TableSelected.push_back(Table[id]);
        }



        if (TableSelected.size() > 0) {
            if (Table[id].b == BUILDING) {
                Inspector.SetBuildingsInspector();
                item_current = BuildingsVector[Table[id].index].Wall_texture;
                cz = BuildingsVector[Table[id].index].z;
                cw = BuildingsVector[Table[id].index].w;
                SelectColors[0] = BuildingsVector[Table[id].index].Color[0][0];
                SelectColor = BuildingsVector[Table[id].index].SelectColor;
            }
            else if (Table[id].b == TERRA)
            {
                Inspector.SetTerraInspector();
            }
            else if (Table[id].b == RODS)
            {
                Inspector.SetRodsInspector();
                //SelectColors[0] = RodsVector[Table[id].index].Color[0];
                for (int i = 0; i < RodsVector[Table[id].index].Mesh.size(); i++) {
                    for (int j = 0; j < RodsVector[Table[id].index].Mesh[i].size(); j++) {
                        //RodsBuff.fillArr(RodsBuff.vertexbuffer, glm::vec3{ 0,0,0 }, RodsVector[Table[id].index].Mesh[i][j]->p);
                    }
                }
            }
            else if (Table[id].b == CRODS)
            {
                Inspector.SetCRodsInspector();
                SelectColors[0] = CRodsVector[Table[id].index].Color[0][0];
                SelectColors[2] = CRodsVector[Table[id].index].Color[1][0];
            }
            else if (Table[id].b == POWERLINE)
            {
                Inspector.SetPLInspector();
                SelectColors[0] = PLVector[Table[id].index].Color[0][0];
                SelectColors[2] = PLVector[Table[id].index].Color[1][0];
            }
            else if (Table[id].b == EBUILDING)
            {
                Inspector.SetEBuildingInspector();
            }
            else if (Table[id].b == TUBE)
            {
                Inspector.SetTubeInspector();
            }
            else if (Table[id].b == BRIDGE)
            {
                Inspector.SetBridgeInspector();
            }
            else if (Table[id].b == ZONE)
            {
                Inspector.SetZoneInspector();
                SelectColors[0] = ZoneVector[Table[id].index].Color[0][0];
            }
            else {
                Inspector.False();
            }
        }
    }
    else {
        if (TableSelected.size() > 0) {
            for (int i = 0; i < TableSelected.size(); i++) {
                Table[TableSelected[i].indexGlobal].selected = false;
                TableSelected.erase(TableSelected.begin() + i);
            }
        }
        Inspector.SetSkyInspector();
    }



    //glm::vec3 Ray = CreateRay();
    /*object_3d RayC;
    Cylinder C = drawCylinder(cameraPos.x , cameraPos.y, cameraPos.z, objcoord.x , objcoord.y , objcoord.z, 1, 1, true, 8);
    for (int i = 0; i < C.vertices.size(); i++)
    {
        RayC.vertices.push_back(C.vertices[i]);
        RayC.colors_backup.push_back(Color3); RayC.colors_alt.push_back(Color3);
        RayC.normals.push_back(C.normals[i]);
        RayC.indices.push_back(i);
    }
    Rays.push_back(RayC);
    int i = Rays.size() - 1;
    glGenVertexArrays(1, &Rays[i].VertexArrayZone);
    glBindVertexArray(Rays[i].VertexArrayZone);

    glGenBuffers(1, &Rays[i].vertexbufferZone);
    glBindBuffer(GL_ARRAY_BUFFER, Rays[i].vertexbufferZone);
    glBufferData(GL_ARRAY_BUFFER, Rays[i].vertices.size() * sizeof(glm::vec3), &Rays[i].vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &Rays[i].colorbufferZone);
    glBindBuffer(GL_ARRAY_BUFFER, Rays[i].colorbufferZone);
    glBufferData(GL_ARRAY_BUFFER, Rays[i].colors_backup.size() * sizeof(glm::vec3), &Rays[i].colors_backup[0], GL_STATIC_DRAW);

    glGenBuffers(1, &Rays[i].colorbuffer_altZone);
    glBindBuffer(GL_ARRAY_BUFFER, Rays[i].colorbuffer_altZone);
    glBufferData(GL_ARRAY_BUFFER, Rays[i].colors_alt.size() * sizeof(glm::vec3), &Rays[i].colors_alt[0], GL_STATIC_DRAW);

    glGenBuffers(1, &Rays[i].normalbufferZone);
    glBindBuffer(GL_ARRAY_BUFFER, Rays[i].normalbufferZone);
    glBufferData(GL_ARRAY_BUFFER, Rays[i].normals.size() * sizeof(glm::vec3), &Rays[i].normals[0], GL_STATIC_DRAW);*/
    MCLick = false;
}

bool ClickPress = false;
void mouse_click(GLFWwindow* glfwWindow, int glfwButton, int glfwAction, int glfwModifierKeys) {
    if (!ShowSettings) {
        if (glfwAction == GLFW_PRESS) {
            ClickPress = true;
        }
        if ((glfwAction == GLFW_RELEASE) && ClickPress) {
            MCLick = true;
            ClickPress = false;
        }
    }
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!ShowSettings) {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
        lastX = xpos;
        lastY = ypos;

        GLfloat sensitivity = 0.05;	// Change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw_main += xoffset;
        pitch_main += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        /*if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;*/

        glm::vec3 front;
        /*front.x = cos(glm::radians(yaw))* cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw))* cos(glm::radians(pitch));*/


        front.x = sin(glm::radians(yaw_main)) * cos(glm::radians(pitch_main));
        front.y = cos(glm::radians(yaw_main)) * cos(glm::radians(pitch_main));
        front.z = sin(glm::radians(pitch_main));


        cameraFront = glm::normalize(front);
    };
}

bool SpacePress = false;

void CameraInput(int winW, int winH) {

    float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    GLfloat cameraSpeed = KeyboardSpeed * deltaTime;

    CP = cameraPos;

    if ((glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)) {
        glfwSetWindowMonitor(window, nullptr, (mode->width - 1366) / 2, (mode->height - 768) / 2, 1366, 768, mode->refreshRate);
        M->MakeShadow = true;
    }
    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)) {
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        M->MakeShadow = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        cameraSpeed = KeyboardSpeedCTRL * deltaTime;
    }
    else cameraSpeed = KeyboardSpeed * deltaTime;

    if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)) {
        cameraPos += cameraSpeed * cameraFront;
    }
    // Move backward
    if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    // Strafe right
    if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    // Strafe left
    if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if (!ShowSettings) {
            PolygonMode = 1;
            PM = 0;
            if (BindMode) {
                if (BindKey1) {
                    M = &MVector[BindedMode];
                    SwapMode();
                    currentMode = BindedMode;
                    currentModeLast = BindedMode;
                }
                else {
                    M = &MVector[BindedModeLast];
                    SwapMode();
                    currentMode = BindedModeLast;
                    currentModeLast = BindedModeLast;
                }

            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if (!ShowSettings) {
            PolygonMode = 0;
            PM = 0;
            if (BindMode) {
                if (BindKey2) {
                    M = &MVector[BindedMode];
                    SwapMode();
                    currentMode = BindedMode;
                    currentModeLast = BindedMode;
                }
                else {
                    M = &MVector[BindedModeLast];
                    SwapMode();
                    currentMode = BindedModeLast;
                    currentModeLast = BindedModeLast;
                }
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        if (!ShowSettings) {
            PolygonMode = 2;
            PM = 0;
            if (BindMode) {
                if (BindKey3) {
                    M = &MVector[BindedMode];
                    SwapMode();
                    currentMode = BindedMode;
                    currentModeLast = BindedMode;
                }
                else {
                    M = &MVector[BindedModeLast];
                    SwapMode();
                    currentMode = BindedModeLast;
                    currentModeLast = BindedModeLast;
                }
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        if (!ShowSettings) {
            PolygonMode = 3;
            PM = 1;
            if (BindMode) {
                if (BindKey4) {
                    M = &MVector[BindedMode];
                    SwapMode();
                    currentMode = BindedMode;
                    currentModeLast = BindedMode;
                }
                else {
                    M = &MVector[BindedModeLast];
                    SwapMode();
                    currentMode = BindedModeLast;
                    currentModeLast = BindedModeLast;
                }
            }
        }
    }

    if (!((cameraPos.x > min_x) && (cameraPos.x < max_X))) {
        //cameraPos.x = CP.x;
    }
    if (!((cameraPos.y > min_Y) && (cameraPos.y < max_Y))) {
        //cameraPos.y = CP.y;
    }
    if (cameraPos.z < 0.1) {
        //cameraPos.z = 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        ColorMode = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        ColorMode = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        BorderShow = false;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        BorderShow = true;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        SpacePress = true;
    }
    if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) && (SpacePress)) {
        SpacePress = false;
        if (ShowSettings) {
            firstMouse = true;
            glfwSetCursorPosCallback(window, mouse_callback);
            glfwSetMouseButtonCallback(window, mouse_click);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            ShowSettings = false;
        }
        else {
            ShowSettings = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        /*firstMouse = true;
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetMouseButtonCallback(window, mouse_click);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        ShowSettings = false;*/
    }
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        show_help = true;
    }
    //ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 1000.0f);
    ProjectionMatrix = glm::perspective(45.0f, (GLfloat)winW / (GLfloat)winH, 1.0f, 10000.0f);
    ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    CF = { cameraFront.x,cameraFront.z,-cameraFront.y };//cameraFront;
    CU = { cameraUp.x,cameraUp.z,cameraUp.y };//cameraUp;
    ViewMatrix2 = glm::lookAt(cameraPos, cameraPos + CF, CU);
}


// Вспомогательная функция загрузки 2D-текстур из файла
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// Загрузка текстуры кубической карты из 6 отдельных текстур-граней.
// Порядок:
// +X (правая грань)
// -X (левая грань)
// +Y (верх)
// -Y (низ)
// +Z (фронтальная грань) 
// -Z (тыльная грань)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

FILE* file = fopen("MatrixH.dat", "rb");
bool readFile() {
    string s; // сюда будем класть считанные строки
    if (file == NULL) {
        printf("Impossible to open the file MatrixH.dat !\n");
        getchar();
    }
    int i, j;
    float x, y, z;
    bool MCreate = false;
    int res = 0;
    int LIST_SIZE;
    int IEC_i;
    int IEC_j;
    int Trng;
    int Rod;
    int Crod;
    int PL_s;
    int Borders_size;
    int PS_s;
    int Buildings_size;
    int EBuildings_size;
    int Tubes_size;
    int Bridges_size;
    fread(&LIST_SIZE, sizeof(int), 1, file);
    //fseek(file, sizeof(int), SEEK_SET);
    fread(&IEC_i, sizeof(int), 1, file);
    //fseek(file, 2 * sizeof(int), SEEK_SET);
    fread(&IEC_j, sizeof(int), 1, file);
    //fseek(file, 3 * sizeof(int), SEEK_SET);
    fread(&Trng, sizeof(int), 1, file);
    //fseek(file, 4 * sizeof(int), SEEK_SET);
    fread(&Rod, sizeof(int), 1, file);
    //fseek(file, 5 * sizeof(int), SEEK_SET);
    fread(&Crod, sizeof(int), 1, file);
    //fseek(file, 6 * sizeof(int), SEEK_SET);
    fread(&PL_s, sizeof(int), 1, file);
    //fseek(file, 7 * sizeof(int), SEEK_SET);
    fread(&Borders_size, sizeof(int), 1, file);
    //fseek(file, 8 * sizeof(int), SEEK_SET);
    fread(&PS_s, sizeof(int), 1, file);
    //fseek(file, 9 * sizeof(int), SEEK_SET);
    fread(&Buildings_size, sizeof(int), 1, file);
    //fseek(file, 10 * sizeof(int), SEEK_SET);
    fread(&EBuildings_size, sizeof(int), 1, file);
    //fseek(file, 11 * sizeof(int), SEEK_SET);
    fread(&Tubes_size, sizeof(int), 1, file);
    //fseek(file, 12 * sizeof(int), SEEK_SET);
    fread(&Bridges_size, sizeof(int), 1, file);
    //fseek(file, 13 * sizeof(int), SEEK_SET);
    if ((IEC_i > 0) || (IEC_j > 0)) {
        Matrix NewM(IEC_i, IEC_j);
        MP = NewM;
        for (int i = 0; i < LIST_SIZE; i++) {
            Pack_point P;
            //fseek(file, 10 * sizeof(int) + i * sizeof(Pack_point), SEEK_SET);
            fread(&P, sizeof(Pack_point), 1, file);
            MP.SetItem(P.i, P.j, P.x, P.y * -1, P.z, 0);
        }
        /*for (int i = 0; i < Trng; i++)
        {
            Pack_trng P;
            fseek(file, 10 * sizeof(int) + LIST_SIZE * sizeof(Pack_point) + i * sizeof(Pack_trng), SEEK_SET);
            fread(&P, sizeof(Pack_trng), 1, file);
            P.y1 = P.y1 * -1;
            P.y2 = P.y2 * -1;
            P.y3 = P.y3 * -1;
            Triangles.push_back(P);
        }*/
        RodsVector.reserve(Rod);
        fseek(file, 13 * sizeof(int) + LIST_SIZE * sizeof(Pack_point) + Trng * sizeof(Pack_trng), SEEK_SET);
        for (int i = 0; i < Rod; i++)
        {
            Mesh_type Rod_new;
            Rod_new.n = 2;
            genVectors(Rod_new);
            Pack_Rod P;
            //fseek(file, 10 * sizeof(int) + LIST_SIZE * sizeof(Pack_point) + Trng * sizeof(Pack_trng) + i * sizeof(Pack_Rod), SEEK_SET);
            fread(&P, sizeof(Pack_Rod), 1, file);
            P.y1 = P.y1 * -1;
            Rod_new.Points.push_back(glm::vec3(P.x1, P.y1, P.z1));
            RodsVector.push_back(Rod_new);

        }
        CRodsVector.reserve(Crod);
        for (int i = 0; i < Crod; i++)
        {
            Mesh_type CRod_new;
            CRod_new.n = 3;
            genVectors(CRod_new);
            Pack_CRod P;
            //fseek(file, 10 * sizeof(int) + LIST_SIZE * sizeof(Pack_point) + Trng * sizeof(Pack_trng) + Rod * sizeof(Pack_Rod) + i * sizeof(Pack_CRod), SEEK_SET);
            fread(&P, sizeof(Pack_CRod), 1, file);
            P.y1 = P.y1 * -1;
            P.y2 = P.y2 * -1;
            CRod_new.Points.reserve(2);
            CRod_new.Points.push_back(glm::vec3(P.x1, P.y1, P.z1));
            CRod_new.Points.push_back(glm::vec3(P.x2, P.y2, P.z2));
            CRodsVector.push_back(CRod_new);
        }
        size_t Way = 13 * sizeof(int) + LIST_SIZE * sizeof(Pack_point) + Trng * sizeof(Pack_trng) + Rod * sizeof(Pack_Rod) + Crod * sizeof(Pack_CRod);
        PLVector.reserve(PL_s + PS_s);
        for (int i = 0; i < PL_s; i++)
        {
            //fseek(file, Way, SEEK_SET);
            int PL_size;
            fread(&PL_size, sizeof(int), 1, file);
            //Power_line PL_new;
            Mesh_type PL_new;
            PL_new.n = 3;
            genVectors(PL_new);
            PL_new.Points.reserve(PL_size);
            for (int j = 0; j < PL_size; j++) {
                //fseek(file, Way + sizeof(int) + j * sizeof(Pack_P), SEEK_SET);
                Pack_P P;
                fread(&P, sizeof(Pack_P), 1, file);
                glm::vec3 vertex;
                vertex = { P.x,P.y * -1,P.z };
                PL_new.Points.push_back(vertex);
            }
            PLVector.push_back(PL_new);
            Way = Way + sizeof(int) + PL_size * sizeof(Pack_P);
        }
        /*
        for (int i = 0; i < Borders_size; i++)
        {
            border_pack P;
            //fseek(file, Way + i * sizeof(border_pack), SEEK_SET);
            fread(&P, sizeof(border_pack), 1, file);
            P.y1 = P.y1 * -1;
            P.y2 = P.y2 * -1;
            //Borders.push_back(P);
        }*/
        Way = Way + Borders_size * sizeof(border_pack);
        fseek(file, Way, SEEK_SET);
        for (int i = 0; i < PS_s; i++)
        {
            //fseek(file, Way, SEEK_SET);
            int PL_size;
            fread(&PL_size, sizeof(int), 1, file);
            Mesh_type PL_new;
            PL_new.n = 3;
            genVectors(PL_new);
            PL_new.Points.reserve(PL_size);
            for (int j = 0; j < PL_size; j++) {
                //fseek(file, Way + sizeof(int) + j * sizeof(Pack_P), SEEK_SET);
                Pack_P P;
                fread(&P, sizeof(Pack_P), 1, file);
                glm::vec3 vertex;
                vertex = { P.x,P.y * -1,P.z };
                PL_new.Points.push_back(vertex);

            }
            PLVector.push_back(PL_new);
            Way = Way + sizeof(int) + PL_size * sizeof(Pack_P);
        }
        BuildingsVector.reserve(Buildings_size);
        for (int i = 0; i < Buildings_size; i++)
        {
            //fseek(file, Way, SEEK_SET);
            int Points_s;
            fread(&Points_s, sizeof(int), 1, file);
            //Power_line Build_new;
            BuildingsMesh Build_new;
            Build_new.n = 4;
            genVectors(Build_new);
            Build_new.Points.reserve(Points_s);
            for (int j = 0; j < Points_s; j++) {
                //fseek(file, Way + sizeof(int) + j * sizeof(Pack_P), SEEK_SET);
                Pack_P P;
                fread(&P, sizeof(Pack_P), 1, file);
                glm::vec3 vertex;
                vertex = { P.x,P.y * -1,P.z };
                Build_new.Points.push_back(vertex);

            }
            BuildingsVector.push_back(Build_new);
            Way = Way + sizeof(int) + Points_s * sizeof(Pack_P);
        }
        EBuildingsMeshVector.reserve(EBuildings_size);
        for (int i = 0; i < EBuildings_size; i++) {
            EBuildingsMesh EBM_new;
            EBM_new.n = 1;
            genVectors(EBM_new);
            Ebuilding_pack E_pack;
            fread(&E_pack, sizeof(Ebuilding_pack), 1, file);
            E_pack.y1 = E_pack.y1 * -1;
            E_pack.y2 = E_pack.y2 * -1;
            EBM_new.Points.push_back(E_pack);
            EBuildingsMeshVector.push_back(EBM_new);
            Way = Way + sizeof(Ebuilding_pack);
        }
        TubeMeshVector.reserve(Tubes_size);
        for (int i = 0; i < Tubes_size; i++) {
            TubeMesh TM_new;
            TM_new.n = 1;
            genVectors(TM_new);
            Tube_pack T_pack;
            fread(&T_pack, sizeof(Tube_pack), 1, file);
            T_pack.y = T_pack.y * -1;
            TM_new.Points.push_back(T_pack);
            TubeMeshVector.push_back(TM_new);
            Way = Way + sizeof(Tube_pack);
        }
        BridgeVector.reserve(Bridges_size);
        for (int i = 0; i < Bridges_size; i++)
        {
            //fseek(file, Way, SEEK_SET);
            int Points_s;
            fread(&Points_s, sizeof(int), 1, file);
            Mesh_type Bridge_new;
            Bridge_new.n = 1;
            genVectors(Bridge_new);
            Bridge_new.Points.reserve(Points_s);
            for (int j = 0; j < Points_s; j++) {
                //fseek(file, Way + sizeof(int) + j * sizeof(Pack_P), SEEK_SET);
                Pack_P P;
                fread(&P, sizeof(Pack_P), 1, file);
                glm::vec3 vertex;
                vertex = { P.x,P.y * -1,P.z };
                Bridge_new.Points.push_back(vertex);

            }
            BridgeVector.push_back(Bridge_new);
            Way = Way + sizeof(int) + Points_s * sizeof(Pack_P);
        }
    }
    else {
        printf("Impossible to open the file !\n");
        getchar();
        return -1;
    }
    fclose(file);
}
void input_from_file(string& path)
{
    string s; // сюда будем класть считанные строки
        //std::ifstream file("D:/Project/LigroEngine/LigroEngine/x64/Debug/CommandsList.txt"); // файл из которого читаем (для линукс путь будет выглядеть по другому)
    std::ifstream file_config(path);
    while (getline(file_config, s)) { // пока не достигнут конец файла класть очередную строку в переменную (s)
        //cout << s << endl; // выводим на экран
        InCommand(s);
    }
    file_config.close();
}
void LoadTextures(string& path) {
    string s; // сюда будем класть считанные строки
            //std::ifstream file("D:/Project/LigroEngine/LigroEngine/x64/Debug/CommandsList.txt"); // файл из которого читаем (для линукс путь будет выглядеть по другому)
    std::ifstream file_config(path);
    while (getline(file_config, s)) { // пока не достигнут конец файла класть очередную строку в переменную (s)
        //cout << s << endl; // выводим на экран
        InCommandTextures(s);
    }
    file_config.close();
}

/// <Logs Data>
double lastTime;
int nbFrames;
GLfloat currentTime;
void LogCMD() {
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;

    nbFrames++;
    if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1sec ago
        // printf and reset
        printf("%f ms/frame\n", 1000.0 / double(nbFrames));
        nbFrames = 0;
        lastTime += 1.0;

        cout << "position " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << endl;
        cout << "position + direction " << cameraPos.x + cameraFront.x << " " << cameraPos.y + cameraFront.y << " " << cameraPos.z + cameraFront.z << endl;
        cout << "position " << cameraUp.x << " " << cameraUp.y << " " << cameraUp.z << endl;

    }
}
const double frame_delay = 1.0 / 120.0f; // 60 FPS
double last_render = 0;

vec3 gPosition1(-1.5f, 0.0f, 0.0f);
vec3 gOrientation1;

vec3 gPosition2(1.5f, 0.0f, 0.0f);
quat gOrientation2;


/*void SelectBuilding(BuildingsMesh& B) {

    if ((int)B.Wall_texture != item_current) {
        B.Wall_texture = static_cast<B_Textures>(item_current);
    }
    if ((_CMATH_::abs(B.z - cz) > 0.02) || (_CMATH_::abs(B.w - cw) > 0.02)) {
        B.z = cz;
        B.w = cw;
        B.Resize = true;
    }
    if (SelectColor != B.SelectColor) {
        B.SelectColor = SelectColor;
    }
    if ((_CMATH_::abs(B.Color[0][0].x - SelectColors[0].x) > 0.02) || (_CMATH_::abs(B.Color[0][0].y - SelectColors[0].y) > 0.02) || (_CMATH_::abs(B.Color[0][0].z - SelectColors[0].z) > 0.02)) {
        B.Color[0][0].x = SelectColors[0].x;
        B.Color[0][0].y = SelectColors[0].y;
        B.Color[0][0].z = SelectColors[0].z;
    }
}*/
/*
void SelectRods(RodMesh& B) {

    if ((_CMATH_::abs(B.Color.x - SelectColor3.x) > 0.02) || (_CMATH_::abs(B.Color.y - SelectColor3.y) > 0.02) || (_CMATH_::abs(B.Color.z - SelectColor3.z) > 0.02)) {
        B.Color.x = SelectColor3.x;
        B.Color.y = SelectColor3.y;
        B.Color.z = SelectColor3.z;
    }
}

void  SelectCRods(CRodMesh& B) {
    if ((_CMATH_::abs(B.Color.x - SelectColor3.x) > 0.02) || (_CMATH_::abs(B.Color.y - SelectColor3.y) > 0.02) || (_CMATH_::abs(B.Color.z - SelectColor3.z) > 0.02)) {
        B.Color.x = SelectColor3.x;
        B.Color.y = SelectColor3.y;
        B.Color.z = SelectColor3.z;
    }
    if ((_CMATH_::abs(B.ColorCable.x - SelectColor3_2.x) > 0.02) || (_CMATH_::abs(B.ColorCable.y - SelectColor3_2.y) > 0.02) || (_CMATH_::abs(B.ColorCable.z - SelectColor3_2.z) > 0.02)) {
        B.ColorCable.x = SelectColor3_2.x;
        B.ColorCable.y = SelectColor3_2.y;
        B.ColorCable.z = SelectColor3_2.z;
    }
}

void  SelectPL(PLMesh& B) {
    if ((_CMATH_::abs(B.Color.x - SelectColor3.x) > 0.02) || (_CMATH_::abs(B.Color.y - SelectColor3.y) > 0.02) || (_CMATH_::abs(B.Color.z - SelectColor3.z) > 0.02)) {
        B.Color.x = SelectColor3.x;
        B.Color.y = SelectColor3.y;
        B.Color.z = SelectColor3.z;
    }
    if ((_CMATH_::abs(B.ColorCable.x - SelectColor3_2.x) > 0.02) || (_CMATH_::abs(B.ColorCable.y - SelectColor3_2.y) > 0.02) || (_CMATH_::abs(B.ColorCable.z - SelectColor3_2.z) > 0.02)) {
        B.ColorCable.x = SelectColor3_2.x;
        B.ColorCable.y = SelectColor3_2.y;
        B.ColorCable.z = SelectColor3_2.z;
    }
}

void SelectZone(ZoneMesh& B) {

    if ((_CMATH_::abs(B.Color.x - SelectColor3.x) > 0.02) || (_CMATH_::abs(B.Color.y - SelectColor3.y) > 0.02) || (_CMATH_::abs(B.Color.z - SelectColor3.z) > 0.02)) {
        B.Color.x = SelectColor3.x;
        B.Color.y = SelectColor3.y;
        B.Color.z = SelectColor3.z;
    }
}
*/

bool gLookAtOther = true;

void drawSelected(unsigned int& fbo, GLuint& texture, GLuint& quad_vertexbuffer, MatFBO& Mat) {
    if (TableSelected.size() > 0)
    {
        if ((TableSelected[0].b != ZONE) && (TableSelected[0].b != TERRA)) {
            /*glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
            // copy depth buffer
            GLbitfield mask = GL_DEPTH_BUFFER_BIT;
            glBlitFramebuffer(0, 0, winW, winH, 0, 0, winW, winH, mask, GL_NEAREST);

            // set clear color*/
            // Render to the screen
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glViewport(0, 0, winW, winH);

            int w, h;
            int miplevel = 0;
            glBindTexture(GL_TEXTURE_2D, texture);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

            if ((w != winW) || (h != winH)) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, winW, winH, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
            }
            glClearColor(1, 1, 1, 1);
            // enable depth test
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            // clear color buffer
            glClear(GL_COLOR_BUFFER_BIT);

            // draw silhouette
            Selected.use();
            Selected.setMat4("MVP", Mat.MVP);
            Selected.setVec3("color", glm::vec3(0, 0, 0));
            for (int i = 0; i < TableSelected.size(); i++) {
                unsigned long id = TableSelected[i].index;
                if (TableSelected[i].b == BUILDING) {
                    Inspector.BMesh = &BuildingsVector[id];
                    for (int k = 0; k < BuildingsVector[id].Mesh.size(); k++) {
                        if (k != 1) {
                            for (int j = 0; j < BuildingsVector[id].Mesh[k].size(); j++) {
                                //ModelMatrix = glm::translate(ModelMatrix, RodsVector[id].Center*-1.0f);
                                DrawObject(BuildingsVector[id].Mesh[k][j]);
                                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                                glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[id].Mesh[k][j]->vertices.size());
                            }
                        }
                        disableAtribut();
                    }
                }
                else if (TableSelected[i].b == RODS)
                {
                    Inspector.Mesh = &RodsVector[id];
                    for (int k = 0; k < RodsVector[id].Mesh.size(); k++) {
                        for (int j = 0; j < RodsVector[id].Mesh[k].size(); j++) {
                            //ModelMatrix = glm::translate(ModelMatrix, RodsVector[id].Center*-1.0f);
                            DrawObject(RodsVector[id].Mesh[k][j]);
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glDrawArrays(GL_TRIANGLES, 0, RodsVector[id].Mesh[k][j]->vertices.size());
                        }
                        disableAtribut();
                    }
                    /*int k = 0;

                    for (int k = 0; k < RodsVector[id].Color.size(); k++) {
                        if (!(AreSameVec3(RodsVector[id].Color[k], SelectColors[k]))) {
                            RodsBuff[k].fillArr(RodsBuff[k].colorbuffer, SelectColors[k], RodsVector[id].Mesh[k][0]->p);
                            RodsVector[id].Color[k] = SelectColors[k];
                            RodsVector[id].Mesh[k][0]->PrivateColor = 1.0f;
                            RodsBuff[k].fillArr(RodsBuff[k].boolbuffer, RodsVector[id].Mesh[k][0]->PrivateColor, RodsVector[id].Mesh[k][0]->p);
                        }
                    }*/
                }
                else if (TableSelected[i].b == CRODS)
                {
                    Inspector.Mesh = &CRodsVector[id];
                    for (int k = 0; k < CRodsVector[id].Mesh.size(); k++) {
                        if (k != 1) {
                            for (int j = 0; j < CRodsVector[id].Mesh[k].size(); j++) {
                                //ModelMatrix = glm::translate(ModelMatrix, RodsVector[id].Center*-1.0f);
                                DrawObject(CRodsVector[id].Mesh[k][j]);
                                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                                glDrawArrays(GL_TRIANGLES, 0, CRodsVector[id].Mesh[k][j]->vertices.size());
                            }
                        }
                        disableAtribut();
                    }
                }
                else if (TableSelected[i].b == POWERLINE)
                {
                    Inspector.Mesh = &PLVector[id];
                    for (int k = 0; k < PLVector[id].Mesh.size(); k++) {
                        if (k != 1) {
                            for (int j = 0; j < PLVector[id].Mesh[k].size(); j++) {
                                //ModelMatrix = glm::translate(ModelMatrix, RodsVector[id].Center*-1.0f);
                                DrawObject(PLVector[id].Mesh[k][j]);
                                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                                glDrawArrays(GL_TRIANGLES, 0, PLVector[id].Mesh[k][j]->vertices.size());
                            }
                        }
                        disableAtribut();
                    }
                }
                else if (TableSelected[i].b == EBUILDING)
                {
                    Inspector.Mesh = &EBuildingsMeshVector[id];
                    for (int k = 0; k < EBuildingsMeshVector[id].Mesh.size(); k++) {
                        if (k != 1) {
                            for (int j = 0; j < EBuildingsMeshVector[id].Mesh[k].size(); j++) {
                                //ModelMatrix = glm::translate(ModelMatrix, RodsVector[id].Center*-1.0f);
                                DrawObject(EBuildingsMeshVector[id].Mesh[k][j]);
                                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                                glDrawArrays(GL_TRIANGLES, 0, EBuildingsMeshVector[id].Mesh[k][j]->vertices.size());
                            }
                        }
                        disableAtribut();
                    }
                }
                else if (TableSelected[i].b == BRIDGE)
                {
                    Inspector.Mesh = &BridgeVector[id];
                    for (int k = 0; k < BridgeVector[id].Mesh.size(); k++) {
                        if (k != 1) {
                            for (int j = 0; j < BridgeVector[id].Mesh[k].size(); j++) {
                                //ModelMatrix = glm::translate(ModelMatrix, RodsVector[id].Center*-1.0f);
                                DrawObject(BridgeVector[id].Mesh[k][j]);
                                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                                glDrawArrays(GL_TRIANGLES, 0, BridgeVector[id].Mesh[k][j]->vertices.size());
                            }
                        }
                        disableAtribut();
                    }
                }
                else if (TableSelected[i].b == TUBE)
                {
                    Inspector.Mesh = &TubeMeshVector[id];
                    for (int k = 0; k < TubeMeshVector[id].Mesh.size(); k++) {
                        if (k != 1) {
                            for (int j = 0; j < TubeMeshVector[id].Mesh[k].size(); j++) {
                                //ModelMatrix = glm::translate(ModelMatrix, RodsVector[id].Center*-1.0f);
                                DrawObject(TubeMeshVector[id].Mesh[k][j]);
                                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                                glDrawArrays(GL_TRIANGLES, 0, TubeMeshVector[id].Mesh[k][j]->vertices.size());
                            }
                        }
                        disableAtribut();
                    }
                }
                else {
                }
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Render on the whole framebuffer, complete from the lower left corner to the upper right
            glViewport(0, 0, winW, winH);

            // Clear the screen
            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Use our shader
            renderTextureShader.use();

            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            // Set our "renderedTexture" sampler to use Texture Unit 0


            // 1rst attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
            glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            // Draw the triangles !
            glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

            glDisableVertexAttribArray(0);
        }
    }
}

template <typename T>
void DrawFirstLayer(vector<T>& buff) {
    for (int i = 0; i < buff.size(); i++)
    {
        FirstLayerShader.setVec3("Out_color", buff[i].rgb);
        for (int k = 0; k < buff[i].Mesh.size(); k++) {
            for (int j = 0; j < buff[i].Mesh[k].size(); j++) {
                DrawObject(buff[i].Mesh[k][j]);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glDrawArrays(GL_TRIANGLES, 0, buff[i].Mesh[k][j]->vertices.size());
            }
        }
    }
}

void DepthLayer(glm::mat4& lightSpaceMatrix, MatFBO& Mat, GLuint& quad_vertexbuffer, GLuint& depthMap) {

    simpleDepthShader.use();
    simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    simpleDepthShader.setMat4("model", Mat.ModelMatrix);
    simpleDepthShader.setMat4("MVP", Mat.MVP);
    DrawFirstLayer(RodsVector);
    DrawFirstLayer(CRodsVector);
    DrawFirstLayer(PLVector);
    DrawFirstLayer(BuildingsVector);
    DrawFirstLayer(EBuildingsMeshVector);
    DrawFirstLayer(BridgeVector);
    DrawFirstLayer(TubeMeshVector);
    //DrawFirstLayer(EBuildingsMeshVector);

    //DrawObject(Border);
    //if (Border.show == 1) {
        //FirstLayerShader.setVec3("Out_color", Border.rgb);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glDrawArrays(GL_TRIANGLES, 0, Border.vertices.size());
    //}
    //disableAtribut();

    /*for (int i = 0; i < BuildingsVector.size(); i++)
    {
        for (int k = 0; k < BuildingsVector[i].Mesh.size(); k++) {
            for (int j = 0; j < BuildingsVector[i].Mesh[k].size(); j++) {
                DrawObject(BuildingsVector[i].Mesh[k][j]);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Mesh[k][j]->vertices.size());
            }
        }
        //disableAtribut();
        //disableAtribut();
    }*/

    DrawObject(terraObj);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, terraObj.vertices.size());


    /// </Initite grid>

    disableAtribut();
    /*if (SetShadowsView) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, winW, winH);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        renderTextureDepth.use();
        renderTextureDepth.setFloat("far_plane", far_plane);
        renderTextureDepth.setFloat("near_plane", near_plane);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        // Set our "renderedTexture" sampler to use Texture Unit 0


        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

        glDisableVertexAttribArray(0);
    }*/
}

void FirstLayer(MatFBO& Mat) {
    long counter = 0;

    FirstLayerShader.use();
    FirstLayerShader.setMat4("MVP", Mat.MVP);
    DrawFirstLayer(RodsVector);
    DrawFirstLayer(CRodsVector);
    DrawFirstLayer(PLVector);
    DrawFirstLayer(EBuildingsMeshVector);
    DrawFirstLayer(BridgeVector);
    DrawFirstLayer(TubeMeshVector);
    //DrawFirstLayer(EBuildingsMeshVector);

    //DrawObject(Border);
    //if (Border.show == 1) {
        //FirstLayerShader.setVec3("Out_color", Border.rgb);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glDrawArrays(GL_TRIANGLES, 0, Border.vertices.size());
    //}
    //disableAtribut();

    for (int i = 0; i < BuildingsVector.size(); i++)
    {
        for (int k = 0; k < BuildingsVector[i].Mesh.size(); k++) {
            FirstLayerShader.setVec3("Out_color", BuildingsVector[i].rgb);
            for (int j = 0; j < BuildingsVector[i].Mesh[k].size(); j++) {
                DrawObject(BuildingsVector[i].Mesh[k][j]);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Mesh[k][j]->vertices.size());
            }
        }
        //disableAtribut();
        for (int j = 0; j < BuildingsVector[i].Windows.size(); j++) {
            DrawObject(BuildingsVector[i].Windows[j]);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Windows[j]->size_verticesLI + 1);
        }
        //disableAtribut();
    }
    FirstLayerShader.setVec3("Out_color", terraObj.rgb);
    DrawObject(terraObj);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, terraObj.vertices.size());

    FirstLayerShaderZone.use();
    FirstLayerShaderZone.setMat4("MVP", Mat.MVP);
    if (PolygonMode != 3) {
        for (int i = 0; i < ZoneVector.size(); i++)
        {
            FirstLayerShaderZone.setVec3("Out_color", ZoneVector[i].rgb);
            for (int j = 0; j < ZoneVector[i].Mesh[0].size(); j++) {
                DrawObject(ZoneVector[i].Mesh[0][j]);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glDrawArrays(GL_TRIANGLES, 0, ZoneVector[i].Mesh[0][j]->vertices.size());
            }
            //disableAtribut();
        }
    }


    /// </Initite grid>

    disableAtribut();
}

void DrawBuffs(vector<OnlyBuff>& buff, int start, int end, bool c, bool ca, bool n, bool u, bool b, bool tg, bool btg) {
    for (int i = start; i < end; i++) {
        if (i == 1) {
            if (BorderShow) {
                //ColorShader.setVec3("CUSTOM_COLOR", *buff[i].Color);
                buff[i].DrawObject(c, ca, n, u, b, tg, btg);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glDrawArrays(GL_TRIANGLES, 0, buff[i].size);
                //disableAtribut();
            }
        }
        else {
            //ColorShader.setVec3("CUSTOM_COLOR", *buff[i].Color);
            buff[i].DrawObject(c, ca, n, u, b, tg, btg);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, buff[i].size);
            //disableAtribut();
        }
    }
};

void SecondLayer(GLuint& depthMap, glm::mat4& lightSpaceMatrix, MatFBO& Mat) {

    float bias;
    if (M->OrthographicShadow) {
        bias = M->biasOrthographic;
    }
    else {
        bias = M->biasPerspective;
    }

    if (M->EnableShadows) {
        TerraDepth.use();
        TerraDepth.setMat4("MVP", Mat.MVP);
        TerraDepth.setMat4("M", Mat.ModelMatrix);
        TerraDepth.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        TerraDepth.setVec3("LightPosition_worldspace", M->lightPos);
        TerraDepth.setVec3("LightDirection", M->LightDirection);
        TerraDepth.setFloat("LightPower", M->lightPower);
        TerraDepth.setFloat("Orthographic", M->OrthographicTerraF);
        TerraDepth.setFloat("bias", bias);
        TerraDepth.setInt("textureTerra", 0);
        TerraDepth.setInt("normalmap", 1);
        TerraDepth.setInt("shadowMap", 2);
        //TerraDepth.setInt("textureTerra2", 3);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TerraTexture);
        glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, terra_normal);
        glActiveTexture(GL_TEXTURE0 + 2); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, depthMap);
        //glActiveTexture(GL_TEXTURE0 + 3); // Texture unit 1
        //glBindTexture(GL_TEXTURE_2D, Terra2Txture);
    }
    else {
        Terra.use();
        Terra.setMat4("MVP", Mat.MVP);
        Terra.setMat4("M", Mat.ModelMatrix);
        Terra.setVec3("LightPosition_worldspace", M->lightPos);
        Terra.setVec3("LightDirection", M->LightDirection);
        Terra.setFloat("LightPower", M->lightPower);
        Terra.setFloat("Orthographic", M->OrthographicTerraF);
        Terra.setInt("textureTerra", 0);
        Terra.setInt("normalmap", 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TerraTexture);
        glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, terra_normal);
    }

    DrawObject(terraObj, false, false, false, true, false, false, false);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, terraObj.vertices.size());

    /// </Initite grid>

    disableAtribut();

    //disableAtribut();

    /*ShaderMO.use();
    ShaderMO.setMat4("MVP", MVP);
    ShaderMO.setMat4("M", ModelMatrix);
    ShaderMO.setMat4("V", ViewMatrix);
    ShaderMO.setVec3("LightPosition_worldspace", lightPos);
    ShaderMO.setInt("ColorMode", ColorMode);
    ShaderMO.setInt("PolygonMode", 0);
    for (int i = 0; i < Lightings.size(); i++) {
        DrawObject(Lightings[i], true, true, true, false);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, Lightings[i].vertices.size());

    }
    disableAtribut();*/

    /*MainShader.use();
    MainShader.setMat4("MVP", MVP);
    MainShader.setMat4("M", ModelMatrix);
    MainShader.setMat4("V", ViewMatrix);
    MainShader.setVec3("LightPosition_worldspace", lightPos);
    MainShader.setInt("ColorMode", ColorMode);
    MainShader.setInt("PolygonMode", 0);*/

    /*ColorShader.use();
    ColorShader.setMat4("MVP", MVP);
    ColorShader.setMat4("M", ModelMatrix);
    ColorShader.setMat4("V", ViewMatrix);
    ColorShader.setVec3("LightPosition_worldspace", lightPos);
    ColorShader.setVec3("CUSTOM_COLOR", Color1);
    DrawObject(Border, false, false, true, false);*/

    //disableAtribut();
    /*
    for (int i = 0; i < Rays.size(); i++) {
        DrawObject(Rays[i], true, true, true, false);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, Rays[i].vertices.size());
    }
    disableAtribut();*/


    if (PolygonMode != 3) {
        for (int i = 0; i < ZoneVector.size(); i++)
        {
            for (int j = 0; j < ZoneVector[i].Mesh[0].size(); j++) {
                ColorShaderZone.use();
                ColorShaderZone.setMat4("MVP", Mat.MVP);
                ColorShaderZone.setMat4("M", Mat.ModelMatrix);
                ColorShaderZone.setMat4("V", Mat.ViewMatrix);
                ColorShaderZone.setVec3("LightPosition_worldspace", M->lightPos);
                ColorShaderZone.setVec3("LightDirection", M->LightDirection);
                ColorShaderZone.setFloat("LightPower", M->lightPower);
                ColorShaderZone.setVec3("CUSTOM_COLOR", M->ColorZone);
                ColorShaderZone.setFloat("z", M->ZoneZ);
                DrawObject(ZoneVector[i].Mesh[0][j], false, false, true, false, false, false, false);

                if (PolygonMode == 1) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else if (PolygonMode == 2) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                }
                else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                if (M->IndexVBO) {
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ZoneVector[i].Mesh[0][j]->IndexbufferZone);
                    glDrawElements(
                        GL_TRIANGLES,      // mode
                        ZoneVector[i].Mesh[0][j]->indices.size(),    // count
                        GL_UNSIGNED_INT,   // type
                        (void*)0           // element array buffer offset
                    );
                }
                else {
                    glDrawArrays(GL_TRIANGLES, 0, ZoneVector[i].Mesh[0][j]->vertices.size());
                }
                //glDrawArrays(GL_TRIANGLES, 0, ZoneVector[i].Mesh[0][j]->vertices.size());
                //disableAtribut();
            }

            if (Table[ZoneVector[i].UnIndex].selected) {
                //SelectZone(ZoneVector[i]);
            }
        }
    }
    if (M->EnableShadows) {
        UnionShaderDepth.use();
        UnionShaderDepth.setMat4("MVP", Mat.MVP);
        UnionShaderDepth.setMat4("M", Mat.ModelMatrix);
        UnionShaderDepth.setMat4("V", Mat.ViewMatrix);
        UnionShaderDepth.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        UnionShaderDepth.setVec3("LightPosition_worldspace", M->lightPos);
        UnionShaderDepth.setVec3("LightDirection", M->LightDirection);
        UnionShaderDepth.setFloat("Orthographic", M->OrthographicObjectsF);
        UnionShaderDepth.setFloat("LightPower", M->lightPower);
        UnionShaderDepth.setFloat("bias", bias);
        for (int i = 0; i < Textures.size(); i++) {
            UnionShaderDepth.setInt("ArrTextures[" + to_string(i) + "]", i * 2);
            UnionShaderDepth.setInt("normalmap[" + to_string(i) + "]", i * 2 + 1);
            glActiveTexture(GL_TEXTURE0 + i * 2); // Texture unit 0
            glBindTexture(GL_TEXTURE_2D, Textures[i].Texture);
            glActiveTexture(GL_TEXTURE0 + i * 2 + 1); // Texture unit 0
            glBindTexture(GL_TEXTURE_2D, Textures[i].Normal);
        }
        UnionShaderDepth.setInt("shadowMap", Textures.size()*2);
        glActiveTexture(GL_TEXTURE0 + Textures.size() * 2); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, depthMap);
    }
    else {
        UnionShader.use();
        UnionShader.setMat4("MVP", Mat.MVP);
        UnionShader.setMat4("M", Mat.ModelMatrix);
        UnionShader.setMat4("V", Mat.ViewMatrix);
        UnionShader.setVec3("LightPosition_worldspace", M->lightPos);
        UnionShader.setVec3("LightDirection", M->LightDirection);
        UnionShader.setFloat("Orthographic", M->OrthographicObjectsF);
        UnionShader.setFloat("LightPower", M->lightPower);
        for (int i = 0; i < Textures.size(); i++) {
            UnionShader.setInt("ArrTextures[" + to_string(i) + "]", i * 2);
            UnionShader.setInt("normalmap[" + to_string(i) + "]", i * 2 + 1);
            glActiveTexture(GL_TEXTURE0 + i * 2); // Texture unit 0
            glBindTexture(GL_TEXTURE_2D, Textures[i].Texture);
            glActiveTexture(GL_TEXTURE0 + i * 2 + 1); // Texture unit 0
            glBindTexture(GL_TEXTURE_2D, Textures[i].Normal);
        }
    };


    /*UnionShader.setInt("ArrTextures[0]", 0);
    UnionShader.setInt("normalmap[0]", 1);
    UnionShader.setInt("ArrTextures[1]", 2);
    UnionShader.setInt("normalmap[1]", 3);
    glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, Concrete);
    glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
    glBindTexture(GL_TEXTURE_2D, Concrete_normal);
    //glActiveTexture(GL_TEXTURE0 + 2); // Texture unit 1
    //glBindTexture(GL_TEXTURE_2D, Concrete_height);
    glActiveTexture(GL_TEXTURE0 + 2); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, brickTexture);
    glActiveTexture(GL_TEXTURE0 + 3); // Texture unit 1
    glBindTexture(GL_TEXTURE_2D, brickTexture_normal);
    //glActiveTexture(GL_TEXTURE0 + 2); // Texture unit 1
    //glBindTexture(GL_TEXTURE_2D, brickTexture_height);*/
    DrawBuffs(TubesBuff, 0, TubesBuff.size(), true, false, true, false, false, false, false);
    DrawBuffs(BridgesBuff, 0, BridgesBuff.size(), true, false, true, false, false, false, false);
    DrawBuffs(EBuildingsBuff, 0, EBuildingsBuff.size(), true, false, true, false, false, false, false);
    DrawBuffs(RodsBuff, 0, RodsBuff.size(), true, false, true, true, true, true, true);
    DrawBuffs(CRodsBuff, 0, CRodsBuff.size(), true, false, true, true, true, true, true);
    DrawBuffs(PLBuff, 0, PLBuff.size(), true, false, true, true, true, true, true);
    DrawBuffs(BoxBuff, 1, BoxBuff.size(), true, false, true, true, true, true, true);
    UnionShader.setFloat("Orthographic", M->OrthographicBuildingsF);
    DrawBuffs(BoxBuff, 0, 1, true, false, true, true, true, true, true);

    /*
    if (M->EnableShadows) {
        ColorShaderDepth.use();
        ColorShaderDepth.setMat4("MVP", Mat.MVP);
        ColorShaderDepth.setMat4("M", Mat.ModelMatrix);
        ColorShaderDepth.setMat4("V", Mat.ViewMatrix);
        ColorShaderDepth.setVec3("LightPosition_worldspace", M->lightPos);
        ColorShaderDepth.setFloat("LightPower", M->lightPower);
        ColorShaderDepth.setVec3("LightDirection", M->LightDirection);
        ColorShaderDepth.setFloat("Orthographic", M->OrthographicObjectsF);
        ColorShaderDepth.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        ColorShaderDepth.setInt("shadowMap", 0);
        ColorShaderDepth.setFloat("bias", bias);
        glActiveTexture(GL_TEXTURE0); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, depthMap);
    }
    else {
        ColorShader.use();
        ColorShader.setMat4("MVP", Mat.MVP);
        ColorShader.setMat4("M", Mat.ModelMatrix);
        ColorShader.setMat4("V", Mat.ViewMatrix);
        ColorShader.setVec3("LightPosition_worldspace", M->lightPos);
        ColorShader.setVec3("LightDirection", M->LightDirection);
        ColorShader.setFloat("Orthographic", M->OrthographicObjectsF);
        ColorShader.setFloat("LightPower", M->lightPower);
        ColorShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    }

    DrawBuffs(RodsBuff,0, RodsBuff.size(), true, false, true, false, true, false, false);
    DrawBuffs(CRodsBuff, 0, CRodsBuff.size(), true, false, true, false, true, false, false);
    DrawBuffs(PLBuff, 0, PLBuff.size(), true, false, true, false, true, false, false);
    if (M->EnableShadows) {
        ColorShaderDepth.setFloat("Orthographic", M->OrthographicBuildingsF);
    }
    else {
        ColorShader.setFloat("Orthographic", M->OrthographicBuildingsF);
    }
    DrawBuffs(BoxBuff,1, BoxBuff.size(), true, false, true, false, true, false, false);
    DrawBuffs(BoxBuff, 0, 1, true, false, true, false, true, false, false);
    */
    /* if (!M->EnableShadows) {
         ColorShaderBuildings.use();
         ColorShaderBuildings.setMat4("MVP", Mat.MVP);
         ColorShaderBuildings.setMat4("M", Mat.ModelMatrix);
         ColorShaderBuildings.setMat4("V", Mat.ViewMatrix);
         ColorShaderBuildings.setFloat("LightPower", M->lightPower);
         ColorShaderBuildings.setVec3("LightPosition_worldspace", M->lightPos);
         ColorShaderBuildings.setVec3("LightDirection", M->LightDirection);
         ColorShaderBuildings.setFloat("Orthographic", M->OrthographicBuildingsF);
         ColorShaderBuildings.setFloat("LightPower", M->lightPower);
         for (int i = 0; i < BuildingsVector.size(); i++) {
             for (int j = 0; j < BuildingsVector[i].Mesh[0].size(); j++) {
                 if (BuildingsVector[i].SelectColor) {
                     ColorShaderBuildings.setVec3("CUSTOM_COLOR", BuildingsVector[i].Color[0][0]);
                     DrawObject(BuildingsVector[i].Mesh[0][j], false, false, true, false, false, false, false);
                     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                     glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Mesh[0][j]->vertices.size());

                 }
             }
         }
     }
     else {
         ColorShaderBuildingsDepth.use();
         ColorShaderBuildingsDepth.setMat4("MVP", Mat.MVP);
         ColorShaderBuildingsDepth.setMat4("M", Mat.ModelMatrix);
         ColorShaderBuildingsDepth.setMat4("V", Mat.ViewMatrix);
         ColorShaderBuildingsDepth.setVec3("LightPosition_worldspace", M->lightPos);
         ColorShaderBuildingsDepth.setFloat("LightPower", M->lightPower);
         ColorShaderBuildingsDepth.setVec3("LightDirection", M->LightDirection);
         ColorShaderBuildingsDepth.setFloat("Orthographic", M->OrthographicBuildingsF);
         ColorShaderBuildingsDepth.setMat4("lightSpaceMatrix", lightSpaceMatrix);
         ColorShaderBuildingsDepth.setInt("shadowMap", 0);
         ColorShaderBuildingsDepth.setFloat("bias", bias);
         glActiveTexture(GL_TEXTURE0); // Texture unit 1
         glBindTexture(GL_TEXTURE_2D, depthMap);
         for (int i = 0; i < BuildingsVector.size(); i++) {
             for (int j = 0; j < BuildingsVector[i].Mesh[0].size(); j++) {
                 if (BuildingsVector[i].SelectColor) {
                     ColorShaderBuildingsDepth.setVec3("CUSTOM_COLOR", BuildingsVector[i].Color[0][0]);
                     DrawObject(BuildingsVector[i].Mesh[0][j], false, false, true, false, false, false, false);
                     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                     glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Mesh[0][j]->vertices.size());

                 }
             }
         }
     }
     disableAtribut();
     if (M->EnableShadows) {
         BuildingsShaderDepth.use();
         BuildingsShaderDepth.setMat4("MVP", Mat.MVP);
         BuildingsShaderDepth.setMat4("M", Mat.ModelMatrix);
         BuildingsShaderDepth.setMat4("V", Mat.ViewMatrix);
         BuildingsShaderDepth.setVec3("LightPosition_worldspace", M->lightPos);
         BuildingsShaderDepth.setVec3("LightDirection", M->LightDirection);
         BuildingsShaderDepth.setFloat("LightPower", M->lightPower);
         BuildingsShaderDepth.setMat4("lightSpaceMatrix", lightSpaceMatrix);
         BuildingsShaderDepth.setFloat("Orthographic", M->OrthographicBuildingsF);
         BuildingsShaderDepth.setFloat("bias", bias);
         BuildingsShaderDepth.setInt("ArrTextures[0]", 0);
         BuildingsShaderDepth.setInt("normalmap[0]", 1);
         BuildingsShaderDepth.setInt("ArrTextures[1]", 2);
         BuildingsShaderDepth.setInt("normalmap[1]", 3);
         BuildingsShaderDepth.setInt("shadowMap", 4);
         //BuildingsShaderDepth.setInt("depthMap", 2);
         //BuildingsShaderDepth.setFloat("height_scale", heightScale);
         //BuildingsShaderDepth.setVec3("viewPos", cameraPos);
         glActiveTexture(GL_TEXTURE0 + 4); // Texture unit 1
         glBindTexture(GL_TEXTURE_2D, depthMap);
     }
     else {
         BuildingsShader.use();
         BuildingsShader.setMat4("MVP", Mat.MVP);
         BuildingsShader.setMat4("M", Mat.ModelMatrix);
         BuildingsShader.setMat4("V", Mat.ViewMatrix);
         BuildingsShader.setVec3("LightPosition_worldspace", M->lightPos);
         BuildingsShader.setVec3("LightDirection", M->LightDirection);
         BuildingsShader.setFloat("LightPower", M->lightPower);
         BuildingsShader.setFloat("Orthographic", M->OrthographicBuildingsF);
         BuildingsShader.setInt("ArrTextures[0]", 0);
         BuildingsShader.setInt("normalmap[0]", 1);
         BuildingsShader.setInt("ArrTextures[1]", 2);
         BuildingsShader.setInt("normalmap[1]", 3);
     }

     glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
     glBindTexture(GL_TEXTURE_2D, brickTexture);
     glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
     glBindTexture(GL_TEXTURE_2D, brickTexture_normal);
     //glActiveTexture(GL_TEXTURE0 + 2); // Texture unit 1
     //glBindTexture(GL_TEXTURE_2D, brickTexture_height);
     glActiveTexture(GL_TEXTURE0 + 2); // Texture unit 0
     glBindTexture(GL_TEXTURE_2D, Concrete);
     glActiveTexture(GL_TEXTURE0 + 3); // Texture unit 1
     glBindTexture(GL_TEXTURE_2D, Concrete_normal);
     //glActiveTexture(GL_TEXTURE0 + 2); // Texture unit 1
     //glBindTexture(GL_TEXTURE_2D, Concrete_height);
     BuildingsShader.setInt("tnum", 0);
     BuildingsShaderDepth.setInt("tnum", 0);
     for (int i = 0; i < BuildingsVector.size(); i++) {
         for (int j = 0; j < BuildingsVector[i].Mesh[0].size(); j++) {
             if ((BuildingsVector[i].Wall_texture == BRICK)&& (!BuildingsVector[i].SelectColor)) {
                 DrawObject(BuildingsVector[i].Mesh[0][j], false, false, true, true, false, true, true);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Mesh[0][j]->vertices.size());

             }
         }
     }
     BuildingsShader.setInt("tnum", 1);
     BuildingsShaderDepth.setInt("tnum", 1);
     for (int i = 0; i < BuildingsVector.size(); i++) {
         for (int j = 0; j < BuildingsVector[i].Mesh[0].size(); j++) {
             if ((BuildingsVector[i].Wall_texture == CONCRETE) && (!BuildingsVector[i].SelectColor)) {
                 DrawObject(BuildingsVector[i].Mesh[0][j], false, false, true, true, false, true, true);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Mesh[0][j]->vertices.size());

             }
         }
     }*/
     /*glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
     glBindTexture(GL_TEXTURE_2D, windowTexture);
     glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
     glBindTexture(GL_TEXTURE_2D, windowTexture_normal);
     //glActiveTexture(GL_TEXTURE0 + 2); // Texture unit 1
     //glBindTexture(GL_TEXTURE_2D, windowTexture_height);
     for (int i = 0; i < BuildingsVector.size(); i++) {
         for (int j = 0; j < BuildingsVector[i].Windows.size(); j++) {
             DrawObject(BuildingsVector[i].Windows[j], false, false, true, true, true, true);
             glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
             glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Windows[j]->size_verticesLI + 1);
             disableAtribut();
         }
     }/*
     disableAtribut();


     /*BuildingsShader.use();
     BuildingsShader.setMat4("MVP", MVP);
     BuildingsShader.setMat4("M", ModelMatrix);
     BuildingsShader.setMat4("V", ViewMatrix);
     BuildingsShader.setVec3("LightPosition_worldspace", lightPos);
     BuildingsShader.setInt("brick", 0);
     BuildingsShader.setInt("concrete", 1);
     glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
     glBindTexture(GL_TEXTURE_2D, brickwall);

     glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
     glBindTexture(GL_TEXTURE_2D, brickwall_normal);

     BuildingsShader.setVec3("CUSTOM_COLOR", *BoxBuff[0].Color);
     BoxBuff[0].DrawObject(true, false, true, true, true, true, true);
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     glDrawArrays(GL_TRIANGLES, 0, BoxBuff[0].size);
     disableAtribut();*/
     /*for (int i = 0; i < BuildingsVector.size(); i++)
     {
         if (!BuildingsVector[i].SelectColor) {

             glActiveTexture(GL_TEXTURE0);
             glBindTexture(GL_TEXTURE_2D, windowTexture);
             for (int j = 0; j < BuildingsVector[i].Windows.size(); j++) {
                 DrawObject(BuildingsVector[i].Windows[j], false, false, false, true);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Windows[j]->size_verticesLI + 1);
                 disableAtribut();
             }
         }

     };*/
     /*for (int i = 0; i < rodBuffers.n; i++) {
         ColorShader.setVec3("CUSTOM_COLOR", *rodBuffers.buff[i]->Color);
         DrawObject(rodBuffers.buff[i], false, false, true, false);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glDrawArrays(GL_TRIANGLES, 0, rodBuffers.buff[i]->size_vertices);
         disableAtribut();
     }*/
     /*for (int i = 0; i < CrodBuffers.n; i++) {
         ColorShader.setVec3("CUSTOM_COLOR", *CrodBuffers.buff[i]->Color);
         DrawObject(CrodBuffers.buff[i], false, false, true, false);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glDrawArrays(GL_TRIANGLES, 0, CrodBuffers.buff[i]->size_vertices);
         disableAtribut();
     }*/
     /*for (int i = 0; i < plBuffers.n; i++) {
         ColorShader.setVec3("CUSTOM_COLOR", *plBuffers.buff[i]->Color);
         DrawObject(plBuffers.buff[i], false, false, true, false);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glDrawArrays(GL_TRIANGLES, 0, plBuffers.buff[i]->size_vertices);
         disableAtribut();
     }*/

     /*for (int i = 0; i < CRodsVector.size(); i++)
     {
         if (!Table[CRodsVector[i].UnIndex].selected) {
             ColorShader.use();
             ColorShader.setMat4("MVP", MVP);
             ColorShader.setMat4("M", ModelMatrix);
             ColorShader.setMat4("V", ViewMatrix);
             ColorShader.setVec3("LightPosition_worldspace", lightPos);
             ColorShader.setVec3("CUSTOM_COLOR", CRodsVector[i].Color);
             for (int j = 0; j < CRodsVector[i].MainMesh.size(); j++) {
                 DrawObject(CRodsVector[i].MainMesh[j], false, false, true, false);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, CRodsVector[i].MainMesh[j].vertices.size());
                 disableAtribut();
             }
             ColorShader.setVec3("CUSTOM_COLOR", CRodsVector[i].ColorCable);
             for (int j = 0; j < CRodsVector[i].CableMesh.size(); j++) {
                 DrawObject(CRodsVector[i].CableMesh[j], false, false, true, false);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, CRodsVector[i].CableMesh[j].vertices.size());
                 disableAtribut();
             }

         }
         else {
             SelectCRods(CRodsVector[i]);
             glEnable(GL_BLEND);
             glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
             Selected.use();
             Selected.setMat4("MVP", MVP);
             for (int j = 0; j < CRodsVector[i].MainMesh.size(); j++) {
                 Selected.setVec3("color", vec3(255, 0, 0));
                 DrawObject(CRodsVector[i].MainMesh[j]);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, CRodsVector[i].MainMesh[j].vertices.size());
                 disableAtribut();
             }
             for (int j = 0; j < CRodsVector[i].CableMesh.size(); j++) {
                 Selected.setVec3("color", vec3(255, 0, 0));
                 DrawObject(CRodsVector[i].CableMesh[j]);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, CRodsVector[i].CableMesh[j].vertices.size());
                 disableAtribut();
             }
             glDisable(GL_BLEND);
         }
     }*/

     /*for (int i = 0; i < PLVector.size(); i++)
     {
         if (!Table[PLVector[i].UnIndex].selected) {
             ColorShader.use();
             ColorShader.setMat4("MVP", MVP);
             ColorShader.setMat4("M", ModelMatrix);
             ColorShader.setMat4("V", ViewMatrix);
             ColorShader.setVec3("LightPosition_worldspace", lightPos);
             ColorShader.setVec3("CUSTOM_COLOR", PLVector[i].Color[0]);
             for (int j = 0; j < PLVector[i].Mesh[0].size(); j++) {
                 DrawObject(PLVector[i].Mesh[0][j], false, false, true, false);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, PLVector[i].Mesh[0][j]->vertices.size());
                 disableAtribut();
             }
             ColorShader.setVec3("CUSTOM_COLOR", PLVector[i].Color[1]);
             for (int j = 0; j < PLVector[i].Mesh[1].size(); j++) {
                 DrawObject(PLVector[i].Mesh[1][j], false, false, true, false);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, PLVector[i].Mesh[1][j]->vertices.size());
                 disableAtribut();
             }

         }
         else {
             //SelectPL(PLVector[i]);
             glEnable(GL_BLEND);
             glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
             Selected.use();
             Selected.setMat4("MVP", MVP);
             for (int j = 0; j < PLVector[i].Mesh[0].size(); j++) {
                 Selected.setVec3("color", vec3(255, 0, 0));
                 DrawObject(PLVector[i].Mesh[0][j]);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, PLVector[i].Mesh[0][j]->vertices.size());
                 disableAtribut();
             }
             for (int j = 0; j < PLVector[i].Mesh[1].size(); j++) {
                 Selected.setVec3("color", vec3(255, 0, 0));
                 DrawObject(PLVector[i].Mesh[1][j]);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, PLVector[i].Mesh[1][j]->vertices.size());
                 disableAtribut();
             }
             glDisable(GL_BLEND);
         }
     }*/

     /*for (int i = 0; i < BuildingsVector.size(); i++)
     {
         if (!Table[BuildingsVector[i].UnIndex].selected) {
             if (!BuildingsVector[i].SelectColor) {
                 BuildingsShader.use();
                 BuildingsShader.setMat4("MVP", MVP);
                 BuildingsShader.setMat4("M", ModelMatrix);
                 BuildingsShader.setMat4("V", ViewMatrix);
                 BuildingsShader.setVec3("Out_color", Color_terra);
                 BuildingsShader.setInt("textureBuilding", 0);
                 BuildingsShader.setVec3("LightPosition_worldspace", lightPos);
                 glActiveTexture(GL_TEXTURE0);
                 if (BuildingsVector[i].Wall_texture == CONCRETE) {
                     glBindTexture(GL_TEXTURE_2D, Concrete);
                 }
                 else {
                     glBindTexture(GL_TEXTURE_2D, brickTexture);
                 }

                 for (int j = 0; j < BuildingsVector[i].Mesh[0].size(); j++) {
                     DrawObject(BuildingsVector[i].Mesh[0][j], false, false, false, true);
                     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                     glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Mesh[0][j]->vertices.size());
                     disableAtribut();
                 }
                 glActiveTexture(GL_TEXTURE0);
                 glBindTexture(GL_TEXTURE_2D, windowTexture);
                 for (int j = 0; j < BuildingsVector[i].Windows.size(); j++) {
                     DrawObject(BuildingsVector[i].Windows[j], false, false, false, true);
                     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                     glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Windows[j]->size_verticesLI + 1);
                     disableAtribut();
                 }
             }
             else {
                 ColorShader.use();
                 ColorShader.setMat4("MVP", MVP);
                 ColorShader.setMat4("M", ModelMatrix);
                 ColorShader.setMat4("V", ViewMatrix);
                 ColorShader.setVec3("LightPosition_worldspace", lightPos);
                 ColorShader.setVec3("CUSTOM_COLOR", BuildingsVector[i].Color[0][0]);
                 for (int j = 0; j < BuildingsVector[i].Mesh[0].size(); j++) {
                     DrawObject(BuildingsVector[i].Mesh[0][j],false, false, true, false);
                     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                     glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Mesh[0][j]->vertices.size());
                     disableAtribut();
                 }
             }

         }
         else {
             SelectBuilding(BuildingsVector[i]);
             glEnable(GL_BLEND);
             glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
             Selected.use();
             Selected.setMat4("MVP", MVP);
             for (int j = 0; j < BuildingsVector[i].Mesh[0].size(); j++) {
                 Selected.setVec3("color", vec3(255, 0, 0));
                 DrawObject(BuildingsVector[i].Mesh[0][j]);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Mesh[0][j]->vertices.size());
                 disableAtribut();
             }
             if (BuildingsVector[i].Resize) {
                 object_3dtest* Windows = new object_3dtest(GetWindows2(i, BuildingsVector[i].z, BuildingsVector[i].w));
                 if (Windows->size_verticesLI > 0) {
                     DelBuffs(BuildingsVector[i].Windows);
                     for (int j = 0; j < BuildingsVector[i].Windows.size(); j++) {
                         BuildingsVector[i].Windows[j]->delobject();
                         delete[] BuildingsVector[i].Windows[j];
                     }
                     BuildingsVector[i].Windows.clear();
                     BuildingsVector[i].Windows.push_back(Windows);
                     GenBuffsforDraw(BuildingsVector[i].Windows, false, false, true, true);
                 }
                 BuildingsVector[i].Resize = false;
             }
             for (int j = 0; j < BuildingsVector[i].Windows.size(); j++) {
                 Selected.setVec3("color", vec3(0, 0, 255));
                 DrawObject(BuildingsVector[i].Windows[j]);
                 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                 glDrawArrays(GL_TRIANGLES, 0, BuildingsVector[i].Windows[j]->size_verticesLI + 1);
                 disableAtribut();
             }
             glDisable(GL_BLEND);
         }
     }*/

}

template < typename T, typename T1>
void FillBuffer(T1& buffs, T& VectorM) {
    for (int n = 0; n < buffs.n; n++) {
        int counter = 0;
        for (int i = 0; i < VectorM.size(); i++)
        {
            if (!VectorM[i].lock) {
                for (int j = 0; j < VectorM[i].Mesh[n].size(); j++)
                {
                    counter += VectorM[i].Mesh[n][j]->vertices.size();
                }
            };
        }
        if (counter > 0) {
            buffs.buff[n]->createVertices(counter);
            buffs.buff[n]->createNormals(counter);
            for (int i = 0; i < VectorM.size(); i++)
            {
                if (!VectorM[i].lock) {
                    for (int j = 0; j < VectorM[i].Mesh[n].size(); j++)
                    {
                        buffs.buff[n]->addVertices(VectorM[i].Mesh[n][j]->vertices, VectorM[i].Mesh[n][j]->vertices.size());
                        buffs.buff[n]->addNormals(VectorM[i].Mesh[n][j]->normals, VectorM[i].Mesh[n][j]->normals.size());
                    }
                }
            }
        }
    }
}
template < typename T>
void FillBuffer(vector<OnlyBuff>& buffs, T& VectorM, int start, int end, bool v, bool c, bool ca, bool n, bool u, bool b, bool tg, bool btg) {
    for (int i = start; i < end; i++) {
        int counter = 0;
        for (int k = 0; k < VectorM.size(); k++)
        {
            for (int j = 0; j < VectorM[k].Mesh[i].size(); j++)
            {
                counter += VectorM[k].Mesh[i][j]->vertices.size();
            }
        }

        buffs[i].size = counter;
        buffs[i].GenNULLBuffs(c, ca, n, u, b, tg, btg);

        for (int k = 0; k < VectorM.size(); k++)
        {
            for (int j = 0; j < VectorM[k].Mesh[i].size(); j++)
            {
                VectorM[k].Mesh[i][j]->p = buffs[i].AddtoBuffs(*VectorM[k].Mesh[i][j], v, c, ca, n, u, b, tg, btg);
            }
        };
    };
};


// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void MakeItems() {
    char c = '\0';
    string s(1, c);
    items = "Color";
    items += s;
    for (auto i : Textures) {
        items += i.Name;
        items += s;
    };
    items += s;
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif


    monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);

    window = glfwCreateWindow(winW, winH, "Ligro3d", NULL, NULL);
    //glfwSetWindowMonitor(window, nullptr, 0, 50, winW, winH, 120);
    glfwSetWindowMonitor(window, nullptr, (mode->width - 1366) / 2, (mode->height - 768) / 2, 1366, 768, mode->refreshRate);

    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }


    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_click);

    // Dark blue background
    //glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    //glClearColor(100.0f/255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 0.0f);
    glClearColor(Color0.x, Color0.y, Color0.z, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);


    Color0 = GetColor("DCDCDC"); // Скайбокc
    Color7 = GetColor("0092DF"); // земля
    Color1 = GetColor("DCDCDC"); // Сетка
    Color2 = GetColor("b22222"); // Треугольники
    Color3 = GetColor("808080"); // МО
    Color4 = GetColor("808080"); // Трос
    Color5 = GetColor("808080"); // Опоры ЭП
    Color6 = GetColor("808080"); // Трос ЭП 
    Color_terra = GetColor("24A159");
    Color_board = GetColor("000000");


    Color_alt = GetColor("000000"); // Цвет градиентный

    input_from_file(path);
    SaveConfig(path_backup);
    LoadTextures(path_textures);

    M = &MVector[0];


    if (readFile() == -1) {
        printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        getchar();
        return -1;
    }


    bmin_x = MP.GetItem(0, 0).x;
    bmax_X = MP.GetItem(MP.GetRow() - 1, MP.GetCol() - 1).x;
    bmax_Y = MP.GetItem(0, 0).y;
    bmin_Y = MP.GetItem(MP.GetRow() - 1, MP.GetCol() - 1).y;
    //lightPos.x = (bmin_x + bmax_X) / 2.0f;
    //lightPos.y = (bmin_Y + bmax_Y) / 2.0f;
    //lightPos.z = 400;
    MakeZones(4);





    MainShader = Shader("Shaders/StandardShading.vertexshader", "Shaders/StandardShading.fragmentshader");
    Terra = Shader("Shaders/terra.vs", "Shaders/terra.fs");
    BuildingsShader = Shader("Shaders/Buildings.vs", "Shaders/Buildings.fs");
    ShaderMO = Shader("Shaders/ShaderMO.vertexshader", "Shaders/ShaderMO.fragmentshader");
    FirstLayerShader = Shader("Shaders/FirstLayer.vs", "Shaders/FirstLayer.fs");
    AimShader = Shader("Shaders/aim.vs", "Shaders/aim.fs");
    Selected = Shader("Shaders/Selected.vs", "Shaders/Selected.fs");
    ColorShader = Shader("Shaders/ColorShader.vs", "Shaders/ColorShader.fs");
    renderTextureShader = Shader("Shaders/Passthrough.vertexshader", "Shaders/WobblyTexture.fragmentshader");
    ColorShaderBuildings = Shader("Shaders/ColorShaderBuilding.vs", "Shaders/ColorShaderBuilding.fs");
    ColorShaderZone = Shader("Shaders/ColorShaderZone.vs", "Shaders/ColorShaderZone.fs");
    simpleDepthShader = Shader("Shaders/simpleDepthShader.vs", "Shaders/simpleDepthShader.fs");
    renderTextureDepth = Shader("Shaders/DepthShow.vs", "Shaders/DepthShow.fs");
    TerraDepth = Shader("Shaders/terraDepth.vs", "Shaders/terraDepth.fs");
    ColorShaderDepth = Shader("Shaders/ColorShaderDepth.vs", "Shaders/ColorShaderDepth.fs");
    BuildingsShaderDepth = Shader("Shaders/BuildingsDepth.vs", "Shaders/BuildingsDepth.fs");
    FirstLayerShaderZone = Shader("Shaders/FirstLayerZone.vs", "Shaders/FirstLayerZone.fs");
    ColorShaderBuildingsDepth = Shader("Shaders/ColorShaderBuildingDepth.vs", "Shaders/ColorShaderBuildingDepth.fs");
    UnionShader = Shader("Shaders/UnionShader.vs", "Shaders/UnionShader.fs");
    UnionShaderDepth = Shader("Shaders/UnionShaderDepth.vs", "Shaders/UnionShaderDepth.fs");

    //lightPos = glm::vec3((vertices[0].x + vertices[vertices.size() - 1].x) / 2, (vertices[0].y + vertices[vertices.size() - 1].y) / 2, 400);
    //lightPos = glm::vec3(-100, 0, 250);
    //lightPos = glm::vec3(125, 31.8, 120.5);
    //LightDirection = glm::vec3(-0.3,- 1.0, -1.0);
    float ratio = winW / winH;
    double marginx = 2.0f / winW * 10.0f;
    double marginy = 2.0f / winH * 10.0f;

    const int Cursor = 6;
    glm::vec2  ItemCursor[Cursor]{
        glm::vec2(-marginx,-marginy),
        glm::vec2(-marginx,marginy),
        glm::vec2(marginx,marginy),
        glm::vec2(marginx,marginy),
        glm::vec2(marginx,-marginy),
        glm::vec2(-marginx,-marginy)

    };
    glm::vec2  ItemCursorUv[Cursor]{
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,1),
        glm::vec2(1,1),
        glm::vec2(1,0),
        glm::vec2(0,0)
    };
    AIM.vertices.assign(ItemCursor, ItemCursor + Cursor);
    AIM.uv.assign(ItemCursorUv, ItemCursorUv + Cursor);



    const int Terrasize = 30;
    glm::vec3 terraPlane[Terrasize] = {
        glm::vec3(bmin_x, bmin_Y, -0.03), glm::vec3(bmin_x, bmax_Y, -0.03), glm::vec3(bmax_X, bmin_Y, -0.03),
        glm::vec3(bmax_X, bmax_Y, -0.03), glm::vec3(bmax_X, bmin_Y, -0.03), glm::vec3(bmin_x, bmax_Y, -0.03),
        glm::vec3(bmin_x, bmin_Y, -0.03), glm::vec3(bmax_X, bmin_Y, -0.03), glm::vec3(bmin_x, bmin_Y, -0.5),
        glm::vec3(bmax_X, bmin_Y, -0.03), glm::vec3(bmax_X, bmin_Y, -0.5), glm::vec3(bmin_x, bmin_Y, -0.5),
        glm::vec3(bmin_x, bmax_Y, -0.03), glm::vec3(bmin_x, bmax_Y, -0.5), glm::vec3(bmax_X, bmax_Y, -0.03),
        glm::vec3(bmax_X, bmax_Y, -0.03), glm::vec3(bmin_x, bmax_Y, -0.5), glm::vec3(bmax_X, bmax_Y, -0.5),
        glm::vec3(bmin_x, bmin_Y, -0.03), glm::vec3(bmin_x, bmin_Y, -0.5), glm::vec3(bmin_x, bmax_Y, -0.03),
        glm::vec3(bmin_x, bmax_Y, -0.03), glm::vec3(bmin_x, bmin_Y, -0.5), glm::vec3(bmin_x, bmax_Y, -0.5),
        glm::vec3(bmax_X, bmin_Y, -0.03), glm::vec3(bmax_X, bmax_Y, -0.03), glm::vec3(bmax_X, bmin_Y, -0.5),
        glm::vec3(bmax_X, bmax_Y, -0.03), glm::vec3(bmax_X, bmax_Y, -0.5),glm::vec3(bmax_X, bmin_Y, -0.5)
    };
    glm::vec2 terraUV[Terrasize] = {
        glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 50.0f), glm::vec2(50.0f, 0.0f),
        glm::vec2(50.0f, 50.0f), glm::vec2(50.0f, 0.0f), glm::vec2(0.0f, 50.0f),
        glm::vec2(0.0f, 0.0f), glm::vec2(50.0f, 0.0f), glm::vec2(0.0f, 0.0f),
        glm::vec2(50.0f, 0.0f), glm::vec2(50.0f, 0.0f), glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 50.0f), glm::vec2(0.0f, 50.0f), glm::vec2(50.0f, 50.0f),
        glm::vec2(50.0f, 50.0f), glm::vec2(0.0f, 50.0f), glm::vec2(50.0f, 50.0f),
        glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 50.0f),
        glm::vec2(0.0f, 50.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 50.0f),
        glm::vec2(50.0f, 0.0f), glm::vec2(50.0f, 50.0f), glm::vec2(50.0f, 0.0f),
        glm::vec2(50.0f, 50.0f), glm::vec2(50.0f, 50.0f),glm::vec2(50.0f, 0.0f)
    };
    terraObj.vertices.assign(terraPlane, terraPlane + Terrasize);
    terraObj.uv.assign(terraUV, terraUV + Terrasize);
    terraObj.SetIndexAlt(UniqueID());
    Table.push_back(Object{ terraObj.UnIndex ,0 ,TERRA });

    RodsBuff[0].Color = &M->ColorRods;
    RodsBuff[1].Color = &M->ColorRodsBorder;
    FillBuffer(RodsBuff, RodsVector, 0, RodsBuff.size(), true, true, false, true, true, true, true, true);
    CRodsBuff[0].Color = &M->ColorCRods;
    CRodsBuff[1].Color = &M->ColorCRodsBorder;
    CRodsBuff[2].Color = &M->ColorCRodsCable;
    FillBuffer(CRodsBuff, CRodsVector, 0, CRodsBuff.size(), true, true, false, true, true, true, true, true);
    PLBuff[0].Color = &M->ColorPL;
    PLBuff[1].Color = &M->ColorPLBorder;
    PLBuff[2].Color = &M->ColorPLCable;
    FillBuffer(PLBuff, PLVector, 0, PLBuff.size(), true, true, false, true, true, true, true, true);
    BoxBuff[0].Color = &M->ColorBuildingBox;
    BoxBuff[1].Color = &M->ColorBuildingBorder;
    BoxBuff[2].Color = &M->ColorBuildingRoof;
    BoxBuff[3].Color = &M->ColorBuildingFloor;
    BoxBuff[0].ColorMode = &M->BuldingsColorMode[0];
    BoxBuff[1].ColorMode = &M->BuldingsColorMode[1];
    BoxBuff[2].ColorMode = &M->BuldingsColorMode[2];
    BoxBuff[3].ColorMode = &M->BuldingsColorMode[3];
    FillBuffer(BoxBuff, BuildingsVector, 0, 1, true, true, false, true, true, true, true, true);
    FillBuffer(BoxBuff, BuildingsVector, 1, BoxBuff.size(), true, true, false, true, true, true, true, true);
    EBuildingsBuff[0].Color = &M->ColorEBuilding;
    FillBuffer(EBuildingsBuff, EBuildingsMeshVector, 0, 1, true, true, false, true, false, false, false, false);
    BridgesBuff[0].Color = &M->ColorBridge;
    FillBuffer(BridgesBuff, BridgeVector, 0, 1, true, true, false, true, false, false, false, false);
    TubesBuff[0].Color = &M->ColorTube;
    FillBuffer(TubesBuff, TubeMeshVector, 0, 1, true, true, false, true, false, false, false, false);
    float value;
    for (int i = 0; i < BuildingsVector.size(); i++) {
        BuildingsVector[i].Wall_texture = static_cast<B_Textures>(M->item_Building);
    }


    static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
    };

    GLuint quad_vertexbuffer;
    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

    GenBuffsforDraw(terraObj, false, false, false, true, false, false, false, false);
    //GenBuffsforDraw(Lightings, true, true, true, false);
    //GenBuffsforDraw(Border, false, false, true, false);
    //GenBuffsforDraw(Zones, true, true, true, false);
    GenBuffsforDraw(AIM);
    for (int i = 0; i < ZoneVector.size(); i++) {
        if (M->IndexVBO) {
            GenBuffsforDraw(ZoneVector[i].Mesh[0], false, false, true, false, false, false, false, true);
        }
        else {
            GenBuffsforDraw(ZoneVector[i].Mesh[0], false, false, true, false, false, false, false, false);
        }
    }

    for (int i = 0; i < RodsVector.size(); i++) {
        GenBuffsforDraw(RodsVector[i].Mesh[0], false, false, true, false, false, false, false, false);
        GenBuffsforDraw(RodsVector[i].Mesh[1], false, false, true, false, false, false, false, false);
    }
    for (int i = 0; i < CRodsVector.size(); i++) {
        GenBuffsforDraw(CRodsVector[i].Mesh[0], false, false, true, false, false, false, false, false);
        GenBuffsforDraw(CRodsVector[i].Mesh[1], false, false, true, false, false, false, false, false);
        GenBuffsforDraw(CRodsVector[i].Mesh[2], false, false, true, false, false, false, false, false);
    }
    for (int i = 0; i < PLVector.size(); i++) {
        GenBuffsforDraw(PLVector[i].Mesh[0], false, false, true, false, false, false, false, false);
        GenBuffsforDraw(PLVector[i].Mesh[1], false, false, true, false, false, false, false, false);
        GenBuffsforDraw(PLVector[i].Mesh[2], false, false, true, false, false, false, false, false);
    }
    for (int i = 0; i < BuildingsVector.size(); i++) {
        GenBuffsforDraw(BuildingsVector[i].Mesh[0], false, false, true, true, false, true, true, false);
        GenBuffsforDraw(BuildingsVector[i].Mesh[1], false, false, true, false, false, false, false, false);
        GenBuffsforDraw(BuildingsVector[i].Mesh[2], false, false, true, false, false, false, false, false);
        GenBuffsforDraw(BuildingsVector[i].Mesh[3], false, false, true, false, false, false, false, false);
        GenBuffsforDraw(BuildingsVector[i].Windows, false, false, true, true, true, true);
    }
    for (int i = 0; i < EBuildingsMeshVector.size(); i++) {
        GenBuffsforDraw(EBuildingsMeshVector[i].Mesh[0], false, false, true, false, false, false, false, false);
    }
    for (int i = 0; i < BridgeVector.size(); i++) {
        GenBuffsforDraw(BridgeVector[i].Mesh[0], false, false, true, false, false, false, false, false);
    }
    for (int i = 0; i < TubeMeshVector.size(); i++) {
        GenBuffsforDraw(TubeMeshVector[i].Mesh[0], false, false, true, false, false, false, false, false);
    }
    
    //GenBuffsforDraw(rodBuffers.buff[0], true, false, true, false);
    /*for (int i = 0; i < CrodBuffers.n; i++) {
        GenBuffsforDraw(CrodBuffers.buff[i], false, false, true, false);
    }
    for (int i = 0; i < plBuffers.n; i++) {
        GenBuffsforDraw(plBuffers.buff[i], false, false, true, false);
    }*/



    brickTexture = loadTexture("Textures/brick.jpg");
    windowTexture = loadTexture("Textures/window.png");
    TerraTexture = loadTexture("Textures/ground.jpg");
    aimTexture = loadTexture("Textures/pngwing.png");
    Concrete = loadTexture("Textures/CONCRETE.jpg");
    brickTexture_normal = loadTexture("Textures/brick_normalmap.jpg");
    Concrete_normal = loadTexture("Textures/CONCRETE_normalmap.jpg");
    windowTexture_normal = loadTexture("Textures/window_normalmap.png");
    terra_normal = loadTexture("Textures/terra_normalmap.png");
    brickTexture_height = loadTexture("Textures/brick_disp.png");
    Concrete_height = loadTexture("Textures/CONCRETE_disp.png");
    windowTexture_height = loadTexture("Textures/window_disp.png");
    Terra2Txture = loadTexture("Textures/ground2.jpg");

    lastTime = glfwGetTime();
    nbFrames = 0;
    float Left, Top, Right, Bottom;
    RECT_ Screen;
    Screen.left = -1;
    Screen.right = 1;
    Screen.top = -1;
    Screen.bottom = 1;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    if (Theme == DARK) {
        ImGui::StyleColorsDark();
    }
    else if (Theme == CLASSIC) {
        ImGui::StyleColorsClassic();
    }
    else if (Theme == LIGHT) {
        ImGui::StyleColorsLight();
    }
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

   /* // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);*/
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    unsigned int SHADOW_WIDTH = M->ShadowScale * 4096, SHADOW_HEIGHT = M->ShadowScale * 4096;
    unsigned int depthMap;
    CreateTextureDepth(depthMap, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glm::mat4 lightProjection = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, M->near_plane, M->far_plane);
    glm::mat4 lightView;
    glm::mat4 lightSpaceMatrix;



    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    GLuint OutLineTexture;
    CreateTexture(OutLineTexture, winW, winH);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, OutLineTexture, 0);
    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers);


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    MatFBO Mat;
    Fill();
    M->MakeShadow = true;
    bool OrthographicShadowLast = M->OrthographicShadow;

    MakeItems();

    // Main loop
    do {
        glfwGetWindowSize(window, &winW, &winH);
        currentTime = glfwGetTime();
        if ((currentTime - last_render) > frame_delay)
        {
            last_render = currentTime;


            // Measure speed
            LogCMD();

            Mat.ViewMatrix = getViewMatrix();
            Mat.ProjectionMatrix = getProjectionMatrix();
            Mat.SetMVP();

            // Render to the screen
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, winW, winH); // Render on the whole framebuffer, complete from the lower left corner to the upper right

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

            if (MCLick) {
                glClearColor(255, 255, 255, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                FirstLayer(Mat);
                mouse_clickMakeRay();
                glClearColor(M->ColorSky.x, M->ColorSky.y, M->ColorSky.z, 0.0f);
            }

            // Clear the screen
            glClearColor(M->ColorSky.x, M->ColorSky.y, M->ColorSky.z, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (M->EnableShadows) {
                //FirstLayer();
                if (M->ShadowScaleLast != M->ShadowScale) {
                    SHADOW_WIDTH = M->ShadowScale * 4096;
                    SHADOW_HEIGHT = M->ShadowScale * 4096;
                    M->MakeShadow = true;
                }
                if (M->SetShadowsView) {
                    M->lightPos = cameraPos;
                    M->LightDirection = cameraFront;
                    M->MakeShadow = true;
                }
                lightView = glm::lookAt(M->lightPos,
                    M->lightPos + M->LightDirection,
                    cameraUp);
                if (M->OrthographicShadow) {
                    lightSpaceMatrix = lightProjection * lightView;
                }
                else {
                    lightSpaceMatrix = Mat.ProjectionMatrix * lightView;
                }
                if (OrthographicShadowLast != M->OrthographicShadow) {
                    M->MakeShadow = true;
                }
                if (M->MakeShadow || (currentTime < 5)) {
                    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
                    glBindTexture(GL_TEXTURE_2D, depthMap);
                    if (M->ShadowScaleLast != M->ShadowScale) {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                            SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
                        M->ShadowScaleLast = M->ShadowScale;
                    }
                    glClear(GL_DEPTH_BUFFER_BIT);
                    DepthLayer(lightSpaceMatrix, Mat, quad_vertexbuffer, depthMap);
                    M->MakeShadow = false;
                    OrthographicShadowLast = M->OrthographicShadow;
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glViewport(0, 0, winW, winH);
                }
                //FirstLayer();
            }
            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            SecondLayer(depthMap, lightSpaceMatrix, Mat);


            if (!SelectedDepth) {
                drawSelected(fbo, OutLineTexture, quad_vertexbuffer, Mat);
            }

            if (!ShowSettings) {
                glDisable(GL_CULL_FACE);
                glDisable(GL_DEPTH_TEST);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                AimShader.use();
                AimShader.setInt("textureAIM", 0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, aimTexture);
                DrawObject(AIM);

                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glDrawArrays(GL_TRIANGLES, 0, AIM.vertices.size());
                disableAtribut();
                glDisable(GL_BLEND);
                glEnable(GL_CULL_FACE);
                glEnable(GL_DEPTH_TEST);
            }


            MakeIO();

            // Compute the MVP matrix from keyboard and mouse input
            CameraInput(winW, winH);

            //ImGui_ImplOpenGL3_NewFrame();
            //ImGui_ImplGlfw_NewFrame();
            //ImGui::NewFrame();
            // Draw GUI
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            if (ShowSettings) {
                // Start the Dear ImGui frame
                ShowExampleAppMainMenuBar();
                if (Inspector.BuildingsInspector) {
                    showObjInspectorBuilding();
                }
                else if (Inspector.SkyInspector) {
                    showObjInspectorSky();
                }
                else if (Inspector.RodsInspector) {
                    showObjInspectorRods();
                }
                else if (Inspector.СRodsInspector) {
                    showObjInspectorCRods();
                }
                else if (Inspector.PLInspector) {
                    showObjInspectorPL();
                }
                else if (Inspector.TubeInspector) {
                    showObjInspector("Tube",TubesBuff,Inspector);
                }
                else if (Inspector.EBuildingInspector) {
                    showObjInspector("EBuilding", EBuildingsBuff, Inspector);
                }
                else if (Inspector.BridgeInspector) {
                    showObjInspector("Bridge", BridgesBuff, Inspector);
                }
                else if (Inspector.ZoneInspector) {
                    showObjInspectorZone();
                };
                if (show_help) {
                    ShowHelp(&show_help);
                }
                // Rendering
            }
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            //glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    SaveConfig(path);
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
