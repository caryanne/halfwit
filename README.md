# halfwit
## a "dumb" irc client/bot
halfwit is a bare-bones IRC client implemented in C++. It connects to a desired server, joins a channel, and then outputs all chat messages to stdout. It is designed to remove some of the complications involved with networking & IRC from an external program that is only concerned with carrying out an action upon receipt of a message. In other words, halfwit is really only meant to be used in a scenario where you are piping its output into another program.

## Status of Project
Currently halfwit can connect to desired server and join a channel. It does not filter messages in any way and can not authenticate with a server if it requires that.

## Dependencies
halfwit requires Boost::ASIO for networking operations and TCLAP for command line argument parsing.

## Usage
    $ ./halfwit <arguments> | [target application]
    
