# Building HTTP server from scratch
- `your_program.sh` builds the server and test binaries
## Running locally
```sh
./your_program.sh

# watch mode
watchexec -w src -w include -w tests -r -- bash -c ./your_program.sh


# another terminal
# ping doesn't undertand ports (ICMP), nc is TCP/UDP
nc -vz localhost 4221

curl -v http://localhost:4221

oha http://localhost:4221

./your_program.sh --directory ./tmp/
```

## Unit tests
```sh
./build/test_string_utils
./build/test_http_server
```

## Tips
```sh
# generate tree view of the project
tree -I "build|vcpkg_installed"
```