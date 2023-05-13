#pragma once

#include <iostream>
#include <string> 
#include <format>
#include "./Container.h"

class Object {
public:
    Object() { Object::sm_count++; }
    virtual ~Object() { Object::sm_count--; assert(sm_count >= 0); }
    virtual std::string toString() const = 0;
    static int get_count() { return sm_count; }
protected:
    static int sm_count;
};
int Object::sm_count = 0;

class Task : virtual public Object {
public:
    Task() {}
    virtual ~Task() {}
    std::string toString() const {    
        return std::format("Completed: {} ", m_completed);
    };
    virtual void execute() = 0;
protected:
    bool m_completed = false;
};

class Named : virtual public Object {
public:
    Named(const std::string & name) : m_name(name) {};
    virtual ~Named() {}
    const std::string & getName() const {
        return m_name;
    };
    std::string toString() const {
        return std::format("Name: {} ", m_name);
    };
protected:
    std::string m_name;
};


class TaskWithResult : public Task {
public:
    int get_result() { return m_result; }
    std::string toString() const {
        return std::format("Result: {} ", m_result);
    };
protected:
    int m_result = 0;
};

class AriphmeticTask : public TaskWithResult, public Named  {
public:
    AriphmeticTask(int arg1, int arg2, char operation, const std::string & name = "") :
        Named(name), 
        m_arg1(arg1), 
        m_arg2(arg2), 
        m_operation(operation) 
    {};
    void execute() {
        switch (m_operation) {
        case '+':
            m_result = m_arg1 + m_arg2;
            break;
        case '-':
            m_result = m_arg1 - m_arg2;
            break;
        case '*':
            m_result = m_arg1 * m_arg2;
            break;
        case '/':
            if (!m_arg2) {
                m_error = "Division By Zero";
            }
            else {
                m_result = m_arg1 / m_arg2;
            }
            break;
        default:
            break;
        }
        m_completed = true;
    }
    std::string toString() const {
        return std::format("Ariphmetic Task: {} {} {} ", m_arg1, m_operation, m_arg2)  +
            Named::toString()  +
            TaskWithResult::toString()  +
            Task::toString()  +
            std::format("Error: {}", m_error);
    };

private:
    int m_arg1, m_arg2;
    char m_operation;
    std::string m_error = "";
};

class AddTaskTask : public Task, public Named {
public:
    AddTaskTask(Container<Task *> & m_container, Task * m_task, const std::string & m_name = "") :
        Named(m_name),
        m_container(m_container),
        m_task(m_task)
    {};
    void execute() {
        m_container.push_tail(m_task);
        m_completed = true;
    };
    std::string toString() const {
        return 
            std::format("AddTask ")  +
            Task::toString() +
            Named::toString();
    };
private:
    Container<Task *> & m_container;
    Task * m_task;
};

class GetObjectsInContainerTask : public TaskWithResult, public Named {
public:
    GetObjectsInContainerTask(Container<Task*> & m_container, const std::string & m_name = "") :
        Named(m_name),
        m_container(m_container)
    {};
    void execute() {
        m_result = m_container.get_length();
        m_completed = true;
    }
    std::string toString() const {
        return 
            std::format("Get Objects in Container ")  +
            TaskWithResult::toString() +
            Named::toString();
    };
private:
    Container<Task *> & m_container;
};

class GetTasksWithResultInCont : public TaskWithResult, public Named {
public:
    GetTasksWithResultInCont(Container<Task*> & m_container, const std::string & m_name = "") :
        Named(m_name),
        m_container(m_container)
    {};
    void execute() {
        int res = 0;
        for (Container<Task *>::iterator it = m_container.begin(); it != nullptr; ++it) {
            Task* task = (*it);
            TaskWithResult * taskWithRes = dynamic_cast<TaskWithResult *>(task);
            if (taskWithRes) res++;
        }
        m_completed = true;
        m_result = res;
    };
    std::string toString() const {
        return 
            std::format( "Get Task With Result in Container ")  +
            TaskWithResult::toString() +
            Named::toString();
    };
private:
    Container<Task *> & m_container;
};

template<typename T>
class ClearContainerTask : public Task {
public:
    ClearContainerTask(Container<T> & m_container) :
        m_container(m_container)
    {};
    void execute() {
        m_container.clear();
        m_completed = true;
    }
    std::string toString() const {
        return 
            std::format( "Clear Container " )  +
            Task::toString();
    };
private:
    Container<T> & m_container;

};

class GetAllObjectsTask : public TaskWithResult, public Named {
public:
    GetAllObjectsTask(const std::string& m_name = "") : Named(m_name) {};
    void execute() {
        m_result = Object::get_count();
        m_completed = true;
    };
    std::string toString() const {
        return std::format("Get All Objects ")  +
            TaskWithResult::toString()  +
            Named::toString();
    };
};

