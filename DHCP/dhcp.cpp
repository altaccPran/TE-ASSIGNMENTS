#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

int main() {
    string remote_user = "pict";
    string remote_host = "<neighboring-IPv4-address>";

    // run both commands on the remote host
    string cmd = "ssh " + remote_user + "@" + remote_host +
                 " \"sudo -S apt update && sudo -S apt install -y nasm\"";

    int result = system(cmd.c_str());

    if (result == 0) {
        cout << "NASM installed successfully on remote machine!" << endl;
    } else {
        cout << "Remote installation failed!" << endl;
    }
    return 0;
}
