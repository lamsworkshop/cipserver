#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <chrono>
#include <future>

using namespace std;

string GetLineFromCin() {
  string line;
  getline(cin, line);
  return line;
}

const char *handler(char*,int&);

#define DEVICE "/dev/ttyACM0"
#define BAUDRATE B115200

void writefile(int fd, char *file) {
  ifstream in(file, ios_base::ate | ios_base::binary);
  if (!in) cerr << "Can't open file!" << endl;
  else {
    ifstream::pos_type pos = in.tellg();
    int size = pos;
    char *pChars = new char[size];
    in.seekg(0, ios::beg);
    in.read(pChars, size);
    char ipd[20];
    sprintf(ipd, "+IPD,0,%d:", size);
    int length = strlen(ipd);
    write(fd, ipd, length);
    write(fd, pChars, size);
    in.close();
  }
}

int main() {
  char buffer[100];
  const char *res;
  int n;
  struct termios oldtio, newtio;
  int fd = open(DEVICE, O_RDWR | O_NOCTTY);
  if (fd < 0) { cerr << DEVICE << " fails open" << endl; return -1; }
  tcgetattr(fd, &oldtio);	// save current port settings
  bzero(&newtio, sizeof(newtio));
  newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;
  newtio.c_lflag = 0;
  newtio.c_cc[VTIME] = 1;	// inter-character timer unused
  newtio.c_cc[VMIN] = 0;	// blocking read until 0 chars received
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);
  auto future = async(launch::async, GetLineFromCin);
  while (true) {
    if ((n = read(fd, buffer, 100)))
      if ((res = handler(buffer, n))) {
        write(fd, res, n);
cerr << res << endl;
      }
    if (future.wait_for(chrono::seconds(0)) == future_status::ready) {
      auto line = future.get();
      strcpy(buffer, line.c_str());
      if (!strcmp("end", buffer)) break;
      n = strlen(buffer);
      if (buffer[0] == '"') {
        buffer[n-1] = 0;	// file name = buffer[1:]
        writefile(fd, &buffer[1]);
      } else {
        strcpy(&buffer[n], "\r\n");
        write(fd, buffer, n+2);
      }
      future = async(launch::async, GetLineFromCin);
    }
  }
  tcsetattr(fd, TCSANOW, &newtio);	// restore port settings
  close(fd);
  return 0;
}

