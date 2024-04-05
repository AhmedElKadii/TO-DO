# TO-DO
basePath: the path of the folder which contains the .cpp file.

ClearScreen(): a method used to clear the screen.

ClearFile(path): a method used to clear the contents of a given file path.

SaveFile(text, path): a method used to save a given string of text to a given file path.

ReadFile(path): a method which returns a string of text of the contents of a given file path.

Tasks(path): a method which returns the number of tasks (or history tasks) using a given file path
            , it searches for "[Task" in the file and goes one by one counting how many tasks there are.

Priority(number): a method which returns the amount of asterisk's, which is then used to show the priority of a task.

REFORMAT(text): a helper method used to reformat a given string of text into the default task configuration
               then, returns it.

DELETE(text, taskNumber): a helper method that is used to remove a specific task using a given task number from
                        a given string of text.

CreateTask(): a method used to make a new task, append it and save it to a file.

DeleteTask(taskNumber, path): a method used to delete a task with a given task number and a file path
                            using the helper method DELETE().

GetPoints(): a method that returns the amount of points you have

AddPoints(points): a method that adds a given amount of points to the amount of saved points then saves it.

SetPoints(points): a method that sets the amount of points saved to the given amount and saves it.

CompleteTask(taskNumber): a method that "completes" a task by removing it from the saved data file, adding it
                         to the saved history file and adding 10 points to the saved points using AddPoints().

GetTasks(): a method that returns the tasks as a string.

GetHistory(): a method that returns the history tasks as a string.

ViewTasks(): a method that displays the tasks with options to complete, cycle between and delete tasks
            , receiving input from ncurses.

ViewHistory(): a method that displays the task history with options to cycle between them
            , receiving input from ncurses.

Store(): a method that displays a "shop"/"store" that you can purchase items and things to reward yourself after
        , doing a good job.

        "---------------|SHOP|--------------"
        "-----------------------------------"
        "--·······························--"
        "--··············****·············--"
        "--··············****·············--"
        "--············********···········--"
        "-----------------------------------"
        "-----------------------------------"
        "-----------------------------------"

main(): the default main method used in C++, it contains logic for selecting an option between:

      -Creating a task
      -Viewing a task
      -Viewing task history
      -Store
      -Clearing task histry               
