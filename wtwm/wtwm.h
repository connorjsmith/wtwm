#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "tree.h"
#include "window.h"

/* Needed for WinSock2 */
#pragma comment (lib, "Ws2_32.lib")

/* Return Code Definitions */
#define SOCKET_BUFFER_LENGTH 512
#define ERROR_CODE -1

/* All wtwm functions must accept a [char* array] and [int] count of parameters and return an [int] success/fail code */
typedef int(*wtwm_fn)(std::vector<std::string>& args);

class WindowsTilingWindowManager;

/* make the wtwm function dictionary available everywhere */
extern WindowsTilingWindowManager wtwm;


class WindowsTilingWindowManager {
private:
    std::unordered_map < std::string, wtwm_fn > fn_name_mapping;
    int insert_node(WNode, int desktop_num);
    int detach_node(WNode&);
    int current_node(int desktop_num);
    WNode& get_node(int hWnd);
    std::vector<WTree> trees;
public:
    // Overall constructor/deconstructors
    WindowsTilingWindowManager();
    ~WindowsTilingWindowManager();

    // Loading configurations and starting the window manager threads. Subscribes to monitor/window add/remove events.
    bool init();
    bool load_cfg(std::unordered_map< std::string, std::string>& args);

    // Virtual Desktop/tree Functions, defined in tree.cpp
    int exclude_desktop(std::vector< std::string >& args);
    int include_desktop(std::vector< std::string >& args);
    int create_desktop (std::vector< std::string >& args);
    int destroy_desktop(std::vector< std::string >& args);
    int current_desktop(std::vector< std::string >& args);

    // Node Functions
    int insert_node (std::vector< std::string >& args);
    int detach_node (std::vector< std::string >& args);
    int current_node(std::vector< std::string >& args);

    // Mouse Functions
    int bind_mouse  (std::vector< std::string >& args);
    int unbind_mouse(std::vector< std::string >& args);

};

