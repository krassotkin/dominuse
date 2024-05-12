# dominuse

**Dominuse** (*domain in use*) is a command line tool that allows you to quickly check if a domain is unavailable for registration by checking its IP address.

This tool is a fast and unlimited alternative to WHOIS services for used domains.
So `dominuse` is suitable for bulk checking whether a domain is already taken, with no restrictions on the frequency of requests.

Technically, `dominuse` tries to resolve a domain name to an IP address using the DNS system and tells you the result.

> Be careful! `dominuse` tells you that the domain is definitely not available for registration ("used"). 
> But, if the domain has "is not used" status, you need to check whether it is available for registration using a regular `whois` service.

----
© 2024 Alexander Krassotkin ([krassotkin.com](https://krassotkin.com/)).
