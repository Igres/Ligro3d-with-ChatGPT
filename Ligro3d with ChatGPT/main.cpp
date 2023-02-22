#include <iostream>
#include <vector>

using namespace std;

// ���������� ����� ��� 3D ��������
class Object {
public:
    string name;
    float x, y, z; // ���������� �������
    Object(string name, float x, float y, float z) {
        this->name = name;
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

// ���������� ����� ��� ��������� �����
class SceneEditor {
public:
    vector<Object> objects; // ������ �������� �� �����
    void addObject(Object obj) {
        objects.push_back(obj);
    }
    void editObject(int index, float x, float y, float z) {
        objects[index].x = x;
        objects[index].y = y;
        objects[index].z = z;
    }
    void moveObject(int index, float dx, float dy, float dz) {
        objects[index].x += dx;
        objects[index].y += dy;
        objects[index].z += dz;
    }
    void printScene() {
        for (int i = 0; i < objects.size(); i++) {
            cout << "Object #" << i << ": " << objects[i].name << " (" << objects[i].x << ", " << objects[i].y << ", " << objects[i].z << ")" << endl;
        }
    }
};

int main() {
    // ������� �������� �����
    SceneEditor editor;

    // ��������� ������� �� �����
    Object obj1("Object 1", 0, 0, 0);
    editor.addObject(obj1);
    Object obj2("Object 2", 1, 1, 1);
    editor.addObject(obj2);

    // ������� ������ �������� �� �����
    editor.printScene();

    // ����������� ������ #1
    editor.editObject(0, 2, 2, 2);

    // ���������� ������ #2
    editor.moveObject(1, 1, 1, 1);

    // ������� ����������� ������ �������� �� �����
    editor.printScene();

    return 0;
}
