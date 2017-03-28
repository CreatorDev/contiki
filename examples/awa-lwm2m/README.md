# AwaBootstrap

This app is an LWM2M client using Contiki with TinyDTLS.

By default it will build for a Mikro-Electronica Clicker board. To
build that, you'll need to ensure your build host has the relevant
tools installed. This is usually built from an ubuntu host.

## Running under Docker

The app can also be built to run on a linux host, and can be built
and run under Docker.  The only requirement for this is to have
docker v1.12 (or better) and docker-compose v1.9.0 (or better) installed.
The included [`docker-compose.yml`](docker-compose.yml) has
NAT64 (tayga) and DNS64 (totd) pre-configured. See
[github.com/CreatorDev/docker-nat64](https://github.com/CreatorDev/docker-nat64)
for more details on this setup.

Edit [`docker-compose.yml`](docker-compose.yml) to set your `PSK_IDENTITY`
and `PSK_SECRET_KEY`.  Then:

    $ docker-compose up -d
    $ docker exec -ti awabootstrap_contiki_1 bash

Inside container, first try pinging an IPv4 host by name.  You'll get
back an IPv6 address and the pings should work.

    # ping6 deviceserver.creatordev.io

Then build it:

    # cd /build
    # make
    # ./awa-lwm2m.minimal-net

## Debugging

You can also debug this from your development workstation using vscode. The [`launch.json`](.vscode/launch.json)
file in this repo contains most of what you need, but you'll need to fill in the
following for your system:

| launch.json entry | description |
|---|---|
| `configurations.ssh.host` | a hostname or IP address for your docker host |
| `configurations.ssh.password` | the root password for the container - see below|

The root password is set to a random password on first boot. You can see this
from the docker host by checking the docker logs - run the following on your
docker host:

    $ docker logs awalwm2m_contiki_1 | grep PASS

Also, make sure you've installed the ["recommended"](https://code.visualstudio.com/docs/editor/extension-gallery#_workspace-recommended-extensions)
vscode extensions.

Once that's all in place, you should be able to run this executable just by hitting
`F5` inside vscode.  See also the [other default keyboard shortcuts](https://code.visualstudio.com/docs/customization/keybindings#_debug).

## IPv6 and Contiki for the uninitiated

- RPL is the IPv6 meshing protocol.  It can be disabled in Contiki with

        CFLAGS += -DUIP_CONF_IPV6_RPL=0

- Contiki uses [RFC4862](https://tools.ietf.org/search/rfc4862),
  "IPv6 Stateless Address Autoconfiguration"

- To discover all IP addresses

    ping6 [ -Ieth0 ] ff02::1

## Debug (**_More!_**)

- Uncomment the following line from [`Makefile`](Makefile):

        #CFLAGS += -DVERBOSE_DEBUG=1 -DDEBUG=1

- To get debug from TinyDTLS, edit the end of [`Makefile.tinydtls`](tinydtls/tinydtls/Makefile.tinydtls)
  as follows:

        # This activates debugging support
        #CFLAGS += -DNDEBUG
        tinydtls_src += dtls_debug.c

- To get some more debug from the Contiki IP stack, you might want to set DEBUG
  in the following source files:

        //#define DEBUG DEBUG_NONE
        #define DEBUG DEBUG_FULL

  - [core/net/ipv6/uip-ds6.c](https://github.com/CreatorDev/contiki/blob/b0390e3/core/net/ipv6/uip-ds6.c#L53)
  - [core/net/ipv6/uip-ds6-route.c](https://github.com/CreatorDev/contiki/blob/b0390e3/core/net/ipv6/uip-ds6-route.c#L76)
