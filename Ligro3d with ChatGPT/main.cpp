#include <iostream>
#include <vector>

using namespace std;

// Определяем класс для 3D объектов
class Object {
public:
    string name;
    float x, y, z; // Координаты объекта
    Object(string name, float x, float y, float z) {
        this->name = name;
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

// Определяем класс для редактора сцены
class SceneEditor {
public:
    vector<Object> objects; // Вектор объектов на сцене
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
    // Создаем редактор сцены
    SceneEditor editor;

    // Добавляем объекты на сцену
    Object obj1("Object 1", 0, 0, 0);
    editor.addObject(obj1);
    Object obj2("Object 2", 1, 1, 1);
    editor.addObject(obj2);

    // Выводим список объектов на сцене
    editor.printScene();

    // Редактируем объект #1
    editor.editObject(0, 2, 2, 2);

    // Перемещаем объект #2
    editor.moveObject(1, 1, 1, 1);

    // Выводим обновленный список объектов на сцене
    editor.printScene();

    return 0;
}
