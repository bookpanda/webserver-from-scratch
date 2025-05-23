# Building HTTP server from scratch
- `your_program.sh` builds the server and test binaries
## Testing locally
```sh
./your_program.sh

# another terminal
# ping doesn't undertand ports (ICMP), nc is TCP/UDP
nc -vz localhost 4221

curl -v http://localhost:4221

```

## Unit tests
```sh
./build/test_string_utils
```