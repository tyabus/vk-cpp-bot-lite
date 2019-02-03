#ifndef PY_H_INCLUDED
#define PY_H_INCLUDED

#include <boost/python.hpp>
#include <Python.h>

namespace py = boost::python;

namespace pyF
{
    py::dict toPythonDict(table map);
    table toTable(py::dict dict);
    py::list toPythonList(args a);
    string getTime();

    string vk_send(string method, py::dict params = {}, bool sendtoken = true);
    string net_send(string url, py::dict param = {}, bool post = true);

    int user_get(int id);

	string error();
}

class PyMainThread  // специальный класс для основного потока
{
public:
    PyMainThread()  // нужно создать экземпляр класса в самом начале работы
    {
        Py_Initialize();        // инициализация интерпретатора Python
        PyEval_InitThreads();   // инициализация потоков в Python и механизма GIL

        mGilState = PyGILState_Ensure();     // забираем себе GIL сразу для настройки многопоточности
        mThreadState = PyEval_SaveThread();  // сохраняем состояние главного потока и отпускаем GIL

        // здесь GIL разблокирован для основного интерпретатора Python и ждёт блокировки из других потоков
    }

    ~PyMainThread() // по завершении работы нужно корректно освободит ресурсы интерпретатора
    {
        // здесь GIL должен быть разблокирован для основного интерпретатора

        PyEval_RestoreThread( mThreadState );   // восстанавливаем состояние главного потока и забираем себе GIL
        PyGILState_Release( mGilState );        // отпускаем блокировку GIL с сохранённым состоянием

        Py_Finalize();  // завершает работу как основного интерпретатора, со всеми под-интерпретаторами Python
    }

private:
    PyGILState_STATE mGilState;     // сохранённое состояние GIL
    PyThreadState* mThreadState;    // сохранённое состояние основного потока
};

class PySubThread   // класс для работы в каждом порождённом потоке
{
public:
    PySubThread()   // нужно для инициализации под-интерпретатора Python в самом начале работы потока
    {
        mMainGilState = PyGILState_Ensure();    // забираем блокировку основного интерпретатора
        mOldThreadState = PyThreadState_Get();  // сохраняем текущее состояние порождённого потока
        mNewThreadState = Py_NewInterpreter();  // создаём в потоке под-интерпретатор
        //PyThreadState_Swap( mNewThreadState );  // с этого момента для потока актуален уже под-интерпретатор --
        //mSubThreadState = PyEval_SaveThread();  // не забываем освободить предыдущую блокировку GIL --
        //mSubGilState = PyGILState_Ensure();     // и заблокировать GIL уже для под-интерпретатора --
    }

    ~PySubThread()  // по завершении работы потока нужно корректно освободить ресурсы под-интепретатора Python
    {
        //PyGILState_Release( mSubGilState );         // разблокируем GIL для под-интерпретатора --
        //PyEval_RestoreThread( mSubThreadState );    // восстанавливаем блокировку и состояние потока для основного интерпретатора --
        Py_EndInterpreter( mNewThreadState );       // завершаем работу под-интерпретатора
        PyThreadState_Swap( mOldThreadState );      // возвращаем состояние потока для основного интерпретатора
        PyGILState_Release( mMainGilState );        // освобождаем блокировку GIL для основного интерпретатора
    }

private:
    PyGILState_STATE mMainGilState;     // состояние GIL основного интерпретатора Python
    PyThreadState* mOldThreadState;     // состояние текущего потока для основного интерпретатора
    PyThreadState* mNewThreadState;     // состояние потока для порождённого под-интерпретатора
    //PyThreadState* mSubThreadState;     // сохранённое состояние потока при разблокировке GIL
    //PyGILState_STATE mSubGilState;      // состояние GIL для порождённого под-интерпретатора Python
};

#endif // PY_H_INCLUDED
