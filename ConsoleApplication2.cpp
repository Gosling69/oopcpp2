#include <iostream>
#include <string> 
#include <random>
#include "./Tasks.h"

using std::cout;
using std::cin;
using std::endl;

char getRandomOperator() {
    char operators[4] = { '+', '-', '/', '*' };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 50);
    return operators[distr(gen) % 4];
}

int main()
{

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(10, 20);

    Container<Task *> tasksCont;
    Container<Task *> toAddCont;
    Container<std::string> results;

    for (int i = 0; i < distr(gen); i++) {
        int rndCase = distr(gen) % 5;
        switch (rndCase) {
        case 0:
            tasksCont.push_tail(new Ariphmetic(distr(gen), distr(gen), getRandomOperator(), std::format("Ariphmetic N{}", i)));
            break;
        case 1:
            tasksCont.push_tail(new Ariphmetic(distr(gen), distr(gen), getRandomOperator(), std::format("Also Ariphmetic N{}", i)));
            break;
        case 2:
            tasksCont.push_tail(new Ariphmetic(distr(gen), distr(gen), getRandomOperator()));
            break;
        case 3:
            tasksCont.push_tail(new AddTask(toAddCont, new Ariphmetic(distr(gen), distr(gen), getRandomOperator(), std::format("Add Task N{}", i))));
            break;
        case 4:
            tasksCont.push_tail(new AddTask(toAddCont, new Ariphmetic(distr(gen), distr(gen), getRandomOperator())));
            break;
        default:
            tasksCont.push_tail(new Ariphmetic(distr(gen), distr(gen), getRandomOperator()));
            break;
        }
    }

    ClearContainerTask<std::string> * clearResults = new ClearContainerTask(results);
    ClearContainerTask<Task *> * clearTasks = new ClearContainerTask(tasksCont);
    ClearContainerTask<Task *> * clearToAddCont = new ClearContainerTask(toAddCont);

    GetObjectsInCont * getObjsInCont = new GetObjectsInCont(tasksCont, "Get Objects in Tasks Container");
    getObjsInCont->execute();
    cout << getObjsInCont->toString() << endl << endl;

    GetTasksWithResultInCont * getTasksWithRes = new GetTasksWithResultInCont(tasksCont, "Get Tasks with Result in Tasks Container");
    getTasksWithRes->execute();
    cout << getTasksWithRes->toString() << endl << endl;

    GetAllObjs * getAll = new GetAllObjs("GetAll");
    getAll->execute();
    cout << getAll->toString() << endl << endl;

    int cntLen = tasksCont.get_length();
    for (int i = 0; i < cntLen; i++) {
        Task * task = tasksCont.get_tail();
        tasksCont.remove_tail();
        task->execute();
        results.push_tail(task->toString());
        delete task;
    }

    cout << results << endl << endl;

    int toAddCntLen = toAddCont.get_length();
    for (int i = 0; i < toAddCntLen; i++) {
        Task * task = toAddCont.get_head();
        toAddCont.remove_head();
        delete task;
    }
    
    clearTasks->execute();
    clearResults->execute();
    clearToAddCont->execute();

    delete clearResults;
    delete clearTasks;
    delete getAll;
    delete getTasksWithRes;
    delete getObjsInCont;
    delete clearToAddCont;


    cout << "Total Objects Count: " << Object::get_count() << endl << endl;

}
