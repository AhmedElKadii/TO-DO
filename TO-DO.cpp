#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <regex>
#include <chrono>
#include <thread>
#include <ncurses.h>
#include <iomanip>
#include <algorithm> 


const std::string basePath = "/Users/kaffae/Documents/VSCode/C++/TO-DO List/";

void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ClearFile(const std::string& path)
{
    std::ofstream file(path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Error opening file for clearing: " << path << std::endl;
    }
    file.close();}

void SaveFile(const std::string& text, const std::string& path)
{
    std::ofstream dataFile(path, std::ios::out);
    if (dataFile.is_open()) {
        dataFile << text;
        dataFile.close();
    } else {
        std::cerr << "Error opening file for saving: " << path << std::endl;
    }
}

std::string ReadFile(std::string path)
{
    std::fstream dataFile;
    dataFile.open(path, std::ios::in);
    if(dataFile.is_open())
    {
        std::string line;
        std::string text;
        while(getline(dataFile, line))
        {
            text += line;
        }
        dataFile.close();
        return text;
    }
    else
    {
        return "";
    }
    return "";
}

int Tasks(std::string path)
{
#ifdef _WIN32
    std::string text = ReadFile(path);
#else
    std::string text = ReadFile(path);
#endif
    
    int count = 0;
    std::string::size_type pos = 0;
    while ((pos = text.find("[Task", pos )) != std::string::npos)
    {
        ++count;
        pos += 5;
    }
    return count;
}

std::string Priority(std::string priority = "1")
{
    int priorityInt = std::stoi(priority);
    switch (priorityInt)
    {
        case 1:
            return "*";
            break;
        case 2:
            return "* *";
            break;
        case 3:
            return "* * *";
            break;
        default:
            return "*";
            break;
    }
}

std::string _REFORMAT(std::string text) {
    std::string newText;
    size_t startPos = 0;
    int taskCount = 1;
    size_t pos = text.find("> ");
    while (pos != std::string::npos) {
        text.insert(pos, "\n\n");
        pos = text.find("> ", pos + 3);    }

    while ((startPos = text.find("[Task ", startPos)) != std::string::npos) {
        size_t endPos = text.find("[Task ", startPos + 1);
        if (endPos == std::string::npos) {
            endPos = text.length();
        }

        std::string task = text.substr(startPos, endPos - startPos);

        size_t numPos = task.find(']');
        if (numPos != std::string::npos) {
            std::string newTaskNumber = std::to_string(taskCount++);
            task.replace(6, numPos - 6, newTaskNumber);        }

        if (!newText.empty()) {
            newText += "\n";
        }

        newText += task;

        startPos = endPos;
    }

    if (!newText.empty() && newText.back() == '\n') {
        newText.pop_back();
    }

    return newText;
}

std::string _DELETE(std::string text, std::string taskNumber)
{
    std::string taskTag = "[Task " + taskNumber;
    size_t taskPos = text.find(taskTag);

    if (taskPos != std::string::npos) {

        size_t taskBlockEnd = text.find("[Task ", taskPos + 1);
        
        if (taskBlockEnd == std::string::npos) {
            taskBlockEnd = text.length();
        }

        text.erase(taskPos, taskBlockEnd - taskPos);
    }

    std::string newText;

    for (char c : text) {
        if ((c == '[' || c == '>')) {
            newText += "\n\n";
        }
        newText += c;
    }
    text = newText;
    
    int iTaskNumber = 1;
    size_t startPos = 0;

    while (startPos != std::string::npos) {
        startPos = text.find("[Task ", startPos);
        if (startPos != std::string::npos) {
            startPos += 6;

            size_t endPos = text.find("]", startPos);
            if (endPos != std::string::npos) {
                std::string newTaskNumber = std::to_string(iTaskNumber);

                text.replace(startPos, endPos - startPos, newTaskNumber);

                iTaskNumber++;

                startPos = endPos;
            }
        }

    }

    size_t pos = text.find("\n");
    if (pos != std::string::npos) {
        text.erase(pos, 1);

        pos = text.find("\n");
        if (pos != std::string::npos) {
            text.erase(pos, 1);
        }
    }

    return text;
}

void CreateTask() 
{
    ClearScreen();
    std::string taskName;
    std::cout << "Enter the task name: ";
    std::getline(std::cin, taskName);
    while(taskName.empty() || taskName == " " || taskName == "\n")
    {
        std::cout << "Invalid input. Please enter a task name: ";
        std::getline(std::cin, taskName);
    }

    std::string taskDescription;
    std::cout << "Enter the task description: ";
    std::getline(std::cin, taskDescription);
    while(taskDescription.empty() || taskDescription == " " || taskDescription == "\n")
    {
        std::cout << "Invalid input. Please enter a task description: ";
        std::getline(std::cin, taskDescription);
    }

    std::string taskDeadline;
    std::cout << "Enter the task deadline: ";
    std::getline(std::cin, taskDeadline);
    while(taskDeadline.empty() || taskDeadline == " " || taskDeadline == "\n")
    {
        std::cout << "Invalid input. Please enter a task deadline: ";
        std::getline(std::cin, taskDeadline);
    }

    std::string taskPriority;
    std::cout << "Enter the task priority(1, 2, 3): ";
    std::getline(std::cin, taskPriority);
    while(taskPriority.empty() || taskPriority == " " || taskPriority == "\n" 
          || taskPriority != "1" && taskPriority != "2" && taskPriority != "3")
    {
        std::cout << "Invalid input. Please enter a task priority(1, 2, 3): ";
        std::getline(std::cin, taskPriority);
    }
    std::string taskPath;
#ifdef _WIN32
    taskPath = "data.txt";
#else
    taskPath = ".data.txt";
#endif

    std::string text =  "[Task " + std::to_string(Tasks(taskPath) + 1) + "] ( " + Priority(taskPriority) +
        " )> Name: " + taskName +
        "> Description: " + taskDescription +
        "> Deadline: " + taskDeadline;

#ifdef _WIN32
    text = ReadFile(basePath + "data.txt") + text;
#else
    text = ReadFile(basePath + ".data.txt") + text;
#endif

    std::string toSearchToday = "/today";
    std::string toSearchTomorrow = "/tomorrow";
    auto now = std::chrono::system_clock::now();
    auto tomorrow = now + std::chrono::hours(24);

    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::time_t tomorrow_c = std::chrono::system_clock::to_time_t(tomorrow);

    std::tm* now_tm = std::localtime(&now_c);
    std::tm* tomorrow_tm = std::localtime(&tomorrow_c);

    std::stringstream ssToday, ssTomorrow;
    ssToday << std::put_time(now_tm, "%Y-%m-%d");
    ssTomorrow << std::put_time(tomorrow_tm, "%Y-%m-%d");

    std::string replaceStrToday = ssToday.str();
    std::string replaceStrTomorrow = ssTomorrow.str();

    size_t posToday = text.find(toSearchToday);
    while(posToday != std::string::npos)
    {
        text.replace(posToday, toSearchToday.size(), replaceStrToday);
        posToday = text.find(toSearchToday, posToday + replaceStrToday.size());
    }

    size_t posTomorrow = text.find(toSearchTomorrow);
    while(posTomorrow != std::string::npos)
    {
        text.replace(posTomorrow, toSearchTomorrow.size(), replaceStrTomorrow);
        posTomorrow = text.find(toSearchTomorrow, posTomorrow + replaceStrTomorrow.size());
    }

#ifdef _WIN32
    SaveFile(_REFORMAT(text), basePath + "data.txt");
#else
    SaveFile(_REFORMAT(text), basePath + ".data.txt");
#endif

    ClearScreen();
}

void DeleteTask(const std::string& taskNumber, const std::string& taskPath) {
    std::string text = ReadFile(taskPath);

    std::string taskToFind = "[Task " + taskNumber + "]";

    size_t pos = text.find(taskToFind);

    if (pos != std::string::npos) {
        size_t nextPos = text.find("[Task ", pos + taskToFind.length());
        if (nextPos == std::string::npos) {
            nextPos = text.length();
        }

        std::string deletedText = text.substr(pos, nextPos - pos);

        text.erase(pos, nextPos - pos);

        ClearFile(taskPath);
        SaveFile(_REFORMAT(text), taskPath);

#ifdef _WIN32
        std::string trashPath = basePath + "trash.txt";
#else
        std::string trashPath = basePath + ".trash.txt";
#endif
        std::string trashContent = ReadFile(trashPath);
        SaveFile(_REFORMAT(trashContent + deletedText), trashPath);
    }
}

int GetPoints()
{
#ifdef _WIN32
    std::string text = ReadFile(basePath + "stats.txt");
#else
    std::string text = ReadFile(basePath + ".stats.txt");
#endif

    text.erase(text.find_last_not_of(" \n\r\t") + 1);

    std::string::size_type pos = text.find("Points: ");
    int points = 0;

    if (pos != std::string::npos) {
        pos += 8;

        std::string numberString = text.substr(pos);

        try {

            points = std::stoi(numberString);
        } catch (const std::invalid_argument& e) {
            return 0;
        } catch (const std::out_of_range& e) {
            return 0;
        }
    }
    return points;
}

void AddPoints(int p)
{
    int points = GetPoints() + p;

#ifdef _WIN32
    ClearFile(basePath + "stats.txt");
    SaveFile("Points: " + std::to_string(points), basePath + "stats.txt");
#else
    ClearFile(basePath + ".stats.txt");
    SaveFile("Points: " + std::to_string(points), basePath + ".stats.txt");
#endif
}

void SetPoints(int p)
{
    int points = p;

#ifdef _WIN32
    ClearFile(basePath + "stats.txt");
    SaveFile("Points: " + std::to_string(points), basePath + "stats.txt");
#else
    ClearFile(basePath + ".stats.txt");
    SaveFile("Points: " + std::to_string(points), basePath + ".stats.txt");
#endif
}



void CompleteTask(int taskIndex) {
    ClearScreen();

    std::string text;
#ifdef _WIN32
    text = ReadFile(basePath + "/data.txt");
#else
    text = ReadFile(basePath + "/.data.txt");
#endif

    std::string taskToFind = "[Task " + std::to_string(taskIndex + 1) + "]";
    size_t pos = text.find(taskToFind);
    if (pos != std::string::npos) {
        size_t nextPos = text.find("[Task ", pos + taskToFind.length());
        if (nextPos == std::string::npos) {
            nextPos = text.length();
        }

        std::string taskToComplete = text.substr(pos, nextPos - pos);
        text.erase(pos, nextPos - pos);



#ifdef _WIN32
        taskToComplete = taskToComplete + ReadFile(basePath + "history.txt");
        SaveFile(_REFORMAT(text), basePath + "data.txt");
        SaveFile(_REFORMAT(taskToComplete), basePath + "history.txt");
#else
        taskToComplete = taskToComplete + ReadFile(basePath + ".history.txt");
        SaveFile(_REFORMAT(text), basePath + ".data.txt");
        SaveFile(_REFORMAT(taskToComplete), basePath + ".history.txt");
#endif

        AddPoints(10);
    }
}

std::string GetTasks()
{

#ifdef _WIN32
    std::string text = ReadFile(basePath + "data.txt");
#else
    std::string text = ReadFile(basePath + ".data.txt");
#endif
    return text;
}

std::string GetHistory()
{

#ifdef _WIN32
    std::string text = ReadFile(basePath + "history.txt");
#else
    std::string text = ReadFile(basePath + ".history.txt");
#endif
    return text;
}

std::vector<std::string> SplitTasks(const std::string& input) {
    std::vector<std::string> tasks;
    std::string::size_type startPos = 0;
    std::string::size_type endPos;

    while ((startPos = input.find("[T", startPos)) != std::string::npos) {
        endPos = input.find("[T", startPos + 2);

        if (endPos == std::string::npos) {
            tasks.push_back(input.substr(startPos));
        } else {
            tasks.push_back(input.substr(startPos, endPos - startPos));
        }

        startPos = endPos;
    }

    return tasks;
}

int ViewTasks() {
    initscr();
    timeout(0);
    noecho();
    curs_set(0);
    clear();
    keypad(stdscr, TRUE);

    int index = 0;
    std::string text = GetTasks();
    std::string output = _REFORMAT(text);
    std::vector<std::string> tasks = SplitTasks(output);
    bool shouldBreak = false;
    std::string taskPath;
#ifdef _WIN32
    taskPath = basePath + "data.txt";
#else
    taskPath = basePath + ".data.txt";
#endif
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    while (!shouldBreak) {
        erase();

        if(text == "Error opening file!") {
            mvprintw(0, 0, "Error opening file!\n");
            return -1;
        } else if(tasks.empty()) {
            mvprintw(0, 0, "No tasks found!\n");
            return -1;
        } else if(index >= 0 && index < tasks.size()) {
            mvprintw(0, 0, "%s", tasks[index].c_str());
        }

        mvprintw(max_y - 1, 0, "[Previous Task(A) | Quit(X) | Complete Task(C) | Delete Task(F) | Next Task(D)]");

        int ch = getch();
        if (ch != ERR) {
            switch (ch) {
                case 'S':
                case 's':
                case '>':
                case '.':
                case 'd':
                case 'D':
                    if (index < Tasks(taskPath) - 1) {
                        index++;
                    }
                    break;
                case 'W':
                case 'w':
                case '<':
                case ',':
                case 'a':
                case 'A':
                    if (index > 0) {
                        index--;
                    }
                    break;
                case 'F':
                case 'f':
#ifdef _WIN32
                    DeleteTask(std::to_string(index+1), taskPath);
#else
                    DeleteTask(std::to_string(index+1), taskPath);
#endif
                    shouldBreak = true;
                    break;
                case 'X':
                case 'x':
                    shouldBreak = true;
                    break;
                case 'C':
                case 'c':
                    CompleteTask(index);
                    shouldBreak = true;
                    break;
                case KEY_RESIZE:
                    getmaxyx(stdscr, max_y, max_x);
                    clear();
                    break;
                default:
                    break;
            }

            refresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    endwin();
    return 0;
}

int ViewHistory() {
    initscr();
    keypad(stdscr, TRUE);
    timeout(0);
    noecho();
    curs_set(0);

    int index = 0;

#ifdef _WIN32
    std::string text = ReadFile(basePath + "history.txt");
#else
    std::string text = ReadFile(basePath + ".history.txt");
#endif

    std::string output = _REFORMAT(text);
    std::vector<std::string> historyEntries = SplitTasks(output);
    bool shouldBreak = false;

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    while (!shouldBreak) {
        erase();

        if (text == "Error opening file!") {
            mvprintw(0, 0, "Error opening file!\n");
            return -1;
        } else if (historyEntries.empty()) {
            mvprintw(0, 0, "No history found!\n");
            return -1;
        } else if (index >= 0 && index < historyEntries.size()) {
            mvprintw(0, 0, "%s", historyEntries[index].c_str());
        }

        mvprintw(max_y - 1, 0, "[Previous Task(A) | Quit(X) | Next Task(D)]");

        int ch = getch();
        if (ch != ERR) {
            switch (ch) {
                case 'd':
                case 'D':
                case 's':
                case 'S':
                    if (index < historyEntries.size() - 1) {
                        index++;
                    }
                    break;
                case 'a':
                case 'A':
                case 'w':
                case 'W':
                    if (index > 0) {
                        index--;
                    }
                    break;
                case 'X':
                case 'x':
                    shouldBreak = true;
                    break;
                case KEY_RESIZE:
                    getmaxyx(stdscr, max_y, max_x);
                    break;
                default:
                    break;
            }
            refresh();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
    endwin();
}

void Store() {
    initscr();
    timeout(0);
    noecho();
    curs_set(0);
    bool bought = false;

    const int frameDelay = 1000;
    bool isFrame1 = true;
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    max_x += 2;

    clear();
    mvprintw(max_y / 2, (max_x - strlen("Welcome to the SHOP!")) / 2, "Welcome to the SHOP!");
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    while (true) {
        erase();
        bool shouldBreak = false;

        mvprintw(0, 0, "Points: %d", GetPoints());
        int maxLength = strlen("---------------|SHOP|--------------");
        int startX = std::max(0, (max_x - maxLength) / 2);

        if (isFrame1) {
            mvprintw(max_y/2-4, startX, "---------------|SHOP|--------------");
            mvprintw(max_y/2-3, startX, "-----------------------------------");
            mvprintw(max_y/2-2, startX, "--·······························--");
            mvprintw(max_y/2-1, startX, "--··············****·············--");
            mvprintw(max_y/2,   startX, "--··············****·············--");
            mvprintw(max_y/2+1, startX, "--············********···········--");
            mvprintw(max_y/2+2, startX, "-----------------------------------");
            mvprintw(max_y/2+3, startX, "-----------------------------------");
            mvprintw(max_y/2+4, startX, "-----------------------------------");
        } else {
            mvprintw(max_y/2-4, startX, "---------------|SHOP|--------------");
            mvprintw(max_y/2-3, startX, "-----------------------------------");
            mvprintw(max_y/2-2, startX, "--··············****·············--");
            mvprintw(max_y/2-1, startX, "--··············****·············--");
            mvprintw(max_y/2,   startX, "--············********···········--");
            mvprintw(max_y/2+1, startX, "--············********···········--");
            mvprintw(max_y/2+2, startX, "-----------------------------------");
            mvprintw(max_y/2+3, startX, "-----------------------------------");
            mvprintw(max_y/2+4, startX, "-----------------------------------");
        }

        mvprintw(max_y-3, 0, "1. Coffee [10]");
        mvprintw(max_y-2, 0, "2. Movie Night! [50]");
        mvprintw(max_y-1, 0, "X. Exit Store");

        start_color();
        use_default_colors();
        init_pair(1, COLOR_RED, -1);

        refresh();

        isFrame1 = !isFrame1;


        for (int i = 0; i < frameDelay; i++) {
            int ch = getch();
            if (ch != ERR) {
                move(max_y - 3, 0);
                clrtoeol();

                bool showMessage = false;

                switch (ch) {
                    case '1':
                        move(max_y - 3, 0);
                        clrtoeol();
                        if (GetPoints() >= 10) {
                            clear();
                            mvprintw(max_y / 2, (max_x - strlen("Enjoy your Coffee!")) / 2, "Enjoy your Coffee!");
                            refresh();
                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                            showMessage = true;
                            bought = true;
                            AddPoints(-10);
                        } else {
                            clear();
                            attron(COLOR_PAIR(1));
                            mvprintw(max_y / 2, (max_x - strlen("Not Enough Points!")) / 2, "Not Enough Points!");
                            attroff(COLOR_PAIR(1));
                            showMessage = true;
                            refresh();
                        }
                        break;
                    case '2':
                        move(max_y - 3, 0);
                        clrtoeol();
                        if (GetPoints() >= 50) {
                            clear();
                            mvprintw(max_y / 2, (max_x - strlen("Enjoy the Movies!")) / 2, "Enjoy the Movies!");
                            refresh();
                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                            showMessage = true;
                            bought = true;
                            AddPoints(-50);
                        } else {
                            clear();
                            attron(COLOR_PAIR(1));
                            mvprintw(max_y / 2, (max_x - strlen("Not Enough Points!")) / 2, "Not Enough Points!");
                            attroff(COLOR_PAIR(1));
                            showMessage = true;
                            refresh();
                        }
                        break;
                    case KEY_RESIZE:
                        getmaxyx(stdscr, max_y, max_x);
                        clear();
                        break;
                    case 'x':
                    case 'X':
                        if (!bought)
                        {
                            clear();
                            mvprintw(max_y / 2, (max_x - strlen("Then don't wate my time!")) / 2, 
                                     "Then don't wate my time!");
                            refresh();
                            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        }
                        else
                        {
                            clear();
                            mvprintw(max_y / 2, (max_x - strlen("Come Again :)")) / 2, "Come Again :)");
                            refresh();
                            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                        }
                        shouldBreak = true;
                        break;
                    case '+':
                        AddPoints(10);
                        break;
                    case '_':
                        AddPoints(-10);
                    default:
                        move(max_y - 3, 0);
                        clrtoeol();
                        attron(COLOR_PAIR(1));
                        mvprintw(max_y - 2, 0, "Invalid choice!");
                        attroff(COLOR_PAIR(1));
                        showMessage = true;
                        break;
                }

                if (showMessage) {
                    refresh();
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    move(max_y - 3, 0);
                    clrtoeol();
                    refresh();
                    if (shouldBreak) {
                        break;
                    }
                    i = frameDelay;
                }
            }
            if (shouldBreak) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        endwin();
        if (shouldBreak) break;
    }
}

int main()
{
    ClearScreen();

    clear();
    refresh();
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    printw("Welcome to TO-DO!\n");
    while (true)
    {
        start_color();
        use_default_colors();
        init_pair(1, COLOR_RED, -1);

        cbreak();
        noecho();
        curs_set(0);

        std::string text = GetTasks();
        std::string output = _REFORMAT(text);
        std::vector<std::string> tasks = SplitTasks(output);
        bool validTasks = true;

        std::string historyText = GetHistory();
        std::string historyOutput = _REFORMAT(historyText);
        std::vector<std::string> historyTasks = SplitTasks(historyOutput);
        bool validHistory = true;

        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);

        if(text == "Error opening file!") {
            validTasks = false;
        } else if(tasks.empty()) {
            validTasks = false;
        } else {
            validTasks = true;
        }

        if(historyText == "Error opening file!") {
            validHistory = false;
        } else if(historyTasks.empty()) {
            validHistory = false;
        } else {
            validHistory = true;
        }

        clear();
        char choice;
        mvprintw(max_y / 2-3, (max_x - strlen("Choose an option: \n\n")) / 2, 
                 "Choose an option: \n\n");
        mvprintw(max_y / 2-2, (max_x - strlen("1. Create a task             |(C)\n")) / 2, 
                 "1. Create a task             |(C)\n");
        mvprintw(max_y / 2-1, (max_x - strlen("2. View tasks                |(V)\n")) / 2, 
                 "2. View tasks                |(V)\n");
        mvprintw(max_y / 2, (max_x - strlen("3. Store                     |(S)\n")) / 2, 
                 "3. Store                     |(S)\n");
        mvprintw(max_y / 2+1, (max_x - strlen("4. Task History              |(H)\n")) / 2, 
                 "4. Task History              |(H)\n");
        mvprintw(max_y / 2+2, (max_x - strlen("5. Reset History             |(R)\n")) / 2, 
                 "5. Reset History             |(R)\n");
        mvprintw(max_y / 2+3, (max_x - strlen("0. Exit                      |(X)")) / 2, 
                 "0. Exit                      |(X)");
        choice = toupper(getch());

        switch (choice)
        {
            case 'C':
            case '1':
                endwin();
                CreateTask();
                break;
            case 'V':
            case '2':
                if (!validTasks) {
                    clear();
                    attron(COLOR_PAIR(1));
                    mvprintw(max_y / 2, (max_x - strlen("No Tasks Available!")) / 2, "No Tasks Available!"); 
                    attroff(COLOR_PAIR(1));
                    refresh();
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    break;
                }
                ViewTasks();
                break;
            case 'S':
            case '3':
                Store();
                refresh();
                break;
            case 'H':
            case '4':
                if (!validHistory) {
                    clear();
                    attron(COLOR_PAIR(1));
                    mvprintw(max_y / 2, (max_x - strlen("No History Available!")) / 2, "No History Available!");
                    attroff(COLOR_PAIR(1));
                    refresh();
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    break;
                }
                ViewHistory();
                break;
            case 'R':
            case '5':
#ifdef _WIN32
                ClearFile(basePath + "history.txt");
#else
                ClearFile(basePath + ".history.txt");
#endif
                clear();
                mvprintw(max_y / 2, (max_x - strlen("Histort Cleared!")) / 2, "History Cleared!");
                refresh();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                break;
            case 'X':
            case '0':
                endwin();
                printw("Exiting...\n");
                break;
            case KEY_RESIZE:
                getmaxyx(stdscr, max_y, max_x);
                clear();
                break;
            default:
                break;
        }
        if(choice == 'X' || choice == '0') break;
        refresh();
        doupdate();
    }
    
    endwin();
    return 0;
}
