### GL C/C++ BaseCamp 2022 Task 1 Heorhii Mamonov

WBS to this task - docs/WBS.txt<br>
Uml diagram to this task - docs/Uml diagram.png<br>

The following approaches are required:
- Console application.
- “make” file to compile application.
- Language/libraries: C/C++, usage of additional libraries or 3rd party code is prohibited. For threading, networking should be used platform-depended API. (details can be discussed individually with assigned mentor).
- Application should be cross-compile, e.g. should be compilable on Linux OS (Ubuntu 18.04) and Windows OS (Windows 10) without any code modifications. It's allowed to use C99 and C11. Application logic must be divided from Network and System API.
- Platform dependent code must be in separate module.
- Network dependent code must be in separate module.

12. Client/Server application to search directory on remote HOST.
- client has functionality to search directory by server request.
- server will request search by "directory name"
- client has to replay with "processing" each 500ms of searching
- client has to reply with success or failure.
- success replay has to include full path to the directory.

### How to use

- To compile use make compile.
- To run use make run.
- If you want to launch server enter 'S', if you want to launch client enter 'C'.
- Enter ip4 address and port to start server or to connect to server.

### Commands:

- To send directory to client you need to use command send.<br>
    Syntax : send id directory <br>
- To send directory to all clients you need to use command sendall.<br>
    Syntax : sendall directory <br>
- To see list of connections use command print.<br>
    Syntax : print<br>
- To disconnect client from serve   r use command disconnect.<br>
    Syntax : disconnect id<br>
- To stop server use command stop.<br>
    Syntax : stop<br>


