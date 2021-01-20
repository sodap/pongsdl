SET newProject=%1%
MKDIR W:\%newProject%
robocopy W:\_ProjectTemplate\ W:\%newProject%\ /e