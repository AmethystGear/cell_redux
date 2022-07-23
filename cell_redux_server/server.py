from http.server import SimpleHTTPRequestHandler, HTTPServer, HTTPStatus
import urllib.parse
import os
from os import listdir
from os.path import isfile, join

hostName = 'localhost'
serverPort = 8080
battle_dumps = 'battle_dumps'

def make_path(r_path):  
    ack = 1
    try:
        root = os.path.dirname(__file__)
        rel_path = os.path.join("..", r_path)

        abs_path = os.path.join(root, rel_path)
        os.chdir(abs_path)
        ack = 0
    except Exception as details:
        print('problem to get to the path '+r_path+' (0001) : ' + str(details))
    return ack

class MyServer(SimpleHTTPRequestHandler):
    def do_GET(self):
        print(self.path)
        if self.path == '/battles/':
            battle_files = ' '.join([f for f in listdir(battle_dumps) if isfile(join(battle_dumps, f))])
            self.send_response(200)
            self.end_headers()
            self.wfile.write(battle_files.encode())
        else:
            super().do_GET()

    def do_POST(self):
        if (self.path == '/upload_fighter'):
            content_length = int(self.headers['Content-Length'])
            body = self.rfile.read(content_length)
            fields = urllib.parse.parse_qs(body)

            fighter_file_name = fields[b'fighter_name'][0].decode('utf-8') + '.c'
            with open('all_fighters/' + fighter_file_name, 'w') as fighter_file:
                fighter_file.write(fields[b'fighter_code'][0].decode('utf-8'))

            self.send_response(200)
            self.end_headers()
            self.wfile.write(b'fighter recieved')
        elif (self.path == '/delete_fighter'):
            content_length = int(self.headers['Content-Length'])
            body = self.rfile.read(content_length)
            fields = urllib.parse.parse_qs(body)
            fighter_to_delete = fields[b'fighter_name'][0].decode('utf-8')
            os.remove('all_fighters/' + fighter_to_delete + '.c')
            
            self.send_response(200)
            self.end_headers()
            self.wfile.write(b'fighter removed')
        elif (self.path == '/run_battle'):
            content_length = int(self.headers['Content-Length'])
            body = self.rfile.read(content_length)
            fields = urllib.parse.parse_qs(body)
            fighters = fields[b'fighter_names'][0].decode('utf-8').split(' ')

            os.system('rm fighters/*')

            for fighter in fighters:
                os.system('cp all_fighters/' + fighter + '.c fighters/' + fighter + ".c")

            os.system("cd .. && cd cell_redux_simulation && ./build.sh")
            os.system("cd .. && cd cell_redux_simulation && ./cell_redux")

            self.send_response(200)
            self.end_headers()

            self.wfile.write(b'ran simulation')
        else:
            self.send_error(HTTPStatus.NOT_IMPLEMENTED)

if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")