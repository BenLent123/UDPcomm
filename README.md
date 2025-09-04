# UDPcomm

## Overview
Real time peer-to-peer UDP CLI chat (RTC-p2p-chat) (long ahhh word) via IPv6 and Yggdrasil. Using Yggdrasil you are given a unique IPV6 address and encoded private key which allows for encrypted (NOT ANNONYMOUS!) communication. This works with UDP hole punching so no server, however, both peers need to start roughly at the same time. 

## Requirments
This program uses yggdrasil for the IPV6 mesh network therefore it is required to have this installed and functional!!

CURRENTLY ONLY FOR LINUX!!!!!!!!!!!!!!!!!!!

**Debian/Ubuntu:**
```bash
sudo apt update
sudo apt install yggdrasil
```

**Fedora:**
```bash
sudo dnf install yggdrasil
```

**Arch Linux:**
```bash
sudo pacman -S yggdrasil
```

**Manual install (using wget):**
```bash
wget https://github.com/yggdrasil-network/yggdrasil-go/releases/latest/download/yggdrasil-linux-amd64 -O yggdrasil
chmod +x yggdrasil
sudo mv yggdrasil /usr/local/bin/
```

## Usage
Build the project:
```bash
make
```
Run the executable:
```bash
./comm
```

## Features
- IPv6 via Yggdrasil mesh integration
- UDP holepunching
- encryption via Yggdrasil mesh network
- QR generation using libqrencode
- basic CLI

# IMPORTANT
- this is ipv6 and end2end encrypted HOWEVER not private
- this is a work in progress so this might have issues :(
- Yggdrasil conf has to be created

## License
See [LICENSE.md](LICENSE.md) for MIT License details.
