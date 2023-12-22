# ft_irc Project - IRC Server
This project was completed as part of the 42 school curriculum in collaboration with three fellow students. The IRC server implemented adheres to IRC protocol specifications and facilitates user communication through the Konversation client.
- lolefevr
- mdiamant42

# Compilation Instructions
To compile the project, use the make command. Rules for re and fclean are also defined to rebuild and clean, respectively.

bash
Copy code
`make`
Usage
Once compiled, run the IRC server with the following command:

bash
Copy code
`./ircserv <port> <password>`
By default, the port used is 6667. You can choose a password as per your choice.

- Connect to Konversation and utilize the following commands to interact with the IRC server:

# Implemented Commands
- /JOIN - Join one or multiple channels
  - Prototype: `/JOIN <{#channelName},#channelName2> <{password},password>`
- /KICK - Kick a user from a channel
  - Prototype: `/KICK <userName> :reason or not`
- /TOPIC - Display or set a channel's topic
  - Prototype: `/TOPIC <#channelName> :newTopic`
- /MODE - Modify channel modes (i, t, k, o, l)
  - Prototype: `/MODE <#channelName> <+itkol> or <-itkol> or <+itkol-itkol>`
- /PART - Leave one or multiple channels
  - Prototype: `/PART <#channelName> :reason or not`
- /QUIT - Quit the IRC server
  - Prototype: `/QUIT`
- /PRIVMSG - Send a private message to a user or channel
  - Prototype: `/PRIVMSG <#channelName> or <userName> :message`
