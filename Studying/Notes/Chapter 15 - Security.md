# Chapter 15: Security

Rachel's summary notes

* [Official book slides - Chapter 15](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch15.ppt)

---

# Slide notes

These are the topics covered in the slides, but finding those topics
covered elseware on the internet, such as Wikipedia.

Compared with the official book slides, he did not cover a large amount
of information Security, but did highlight modern bugs.

## Security

* **Secure** - A system is secure if it is only being used and accessed
by those intended, for all circumstances. This is impossible.
* **Intruders/crackers** - Somebody who is attempting to breach the security of the system.
* **Threat** - A potential security violation.
* **Attack** - An attempt to breach the security of a system.

## Threats: Software

### Stack frame layout

### Stack/Buffer overflow

> In software, a stack overflow occurs if the call stack pointer exceeds the stack bound. 
[Wikipedia](https://en.wikipedia.org/wiki/Stack_overflow)

> In computer security and programming, a buffer overflow, or buffer overrun, 
is an anomaly where a program, while writing data to a buffer, 
overruns the buffer's boundary and overwrites adjacent memory locations. 

> Exploiting the behavior of a buffer overflow is a well-known security exploit. 
On many systems, the memory layout of a program, or the system as a whole, is well defined. 
By sending in data designed to cause a buffer overflow, 
it is possible to write into areas known to hold executable code, 
and replace it with malicious code. Buffers are widespread in operating system (OS) code, 
so it is possible to make attacks that perform privilege escalation and gain unlimited access 
to the computer's resources.
[Wikipedia](https://en.wikipedia.org/wiki/Buffer_overflow)

> Programming languages commonly associated with buffer overflows include C and C++,
which provide no built-in protection against accessing or overwriting data in any part 
of memory and do not automatically check that data written to an array (the built-in buffer type) 
is within the boundaries of that array.
Bounds checking can prevent buffer overflows, 
but requires additional code and processing time.
[Wikipedia](https://en.wikipedia.org/wiki/Buffer_overflow)

**The jump to address stored in a register technique**

> The "jump to register" [...] strategy is to overwrite the return pointer 
with something that will cause the program to jump to a known pointer stored 
within a register which points to the controlled buffer and thus the shellcode.
[Wikipedia](https://en.wikipedia.org/wiki/Buffer_overflow#The_jump_to_address_stored_in_a_register_technique)

These overflows are a way that an attacker can gain access,
by unauthorized user or privilege escalation.

**Example from [Wikipedia](https://en.wikipedia.org/wiki/Buffer_overflow#Example)**

> In the following example expressed in C, a program has two data which are adjacent in memory:
an 8-byte-long string buffer, A, and a two-byte big-endian integer, B.

```c
char           A[8] = "";
unsigned short B    = 1979;
```
> Initially, A contains nothing but zero bytes, and B contains the number 1979

<table class="wikitable" style="width:32em; text-align:center;">
<tr>
<th style="white-space:nowrap;">variable name</th>
<th colspan="8" style="background:#ddf;">A</th>
<th colspan="2" style="background:#fdd;">B</th>
</tr>
<tr style="background:#ddf;">
<th>value</th>
<td colspan="8">[<a href="/wiki/Null_string" class="mw-redirect" title="Null string">null string</a>]</td>
<td colspan="2" style="background:#fdd;"><tt>1979</tt></td>
</tr>
<tr style="background:#ddf;">
<th>hex value</th>
<td><tt>00</tt></td>
<td><tt>00</tt></td>
<td><tt>00</tt></td>
<td><tt>00</tt></td>
<td><tt>00</tt></td>
<td><tt>00</tt></td>
<td><tt>00</tt></td>
<td><tt>00</tt></td>
<td style="background:#fdd;"><tt>07</tt></td>
<td style="background:#fdd;"><tt>BB</tt></td>
</tr>
</table>

> Now, the program attempts to store the null-terminated string "excessive" with ASCII encoding in the A buffer.

```c
strcpy(A, "excessive");
```

> "excessive" is 9 characters long and encodes to 10 bytes including the null terminator, but A can take only 8 bytes. By failing to check the length of the string, it also overwrites the value of B:

<table class="wikitable" style="width:32em; text-align:center;">
<tr>
<th style="white-space:nowrap;">variable name</th>
<th colspan="8" style="background:#ddf;">A</th>
<th colspan="2" style="background:#fdd;">B</th>
</tr>
<tr style="background:#ddf;">
<th>value</th>
<td><tt>'e'</tt></td>
<td><tt>'x'</tt></td>
<td><tt>'c'</tt></td>
<td><tt>'e'</tt></td>
<td><tt>'s'</tt></td>
<td><tt>'s'</tt></td>
<td><tt>'i'</tt></td>
<td><tt>'v'</tt></td>
<td colspan="2" style="background:#dbd;"><tt>25856</tt></td>
</tr>
<tr style="background:#ddf;">
<th>hex</th>
<td><tt>65</tt></td>
<td><tt>78</tt></td>
<td><tt>63</tt></td>
<td><tt>65</tt></td>
<td><tt>73</tt></td>
<td><tt>73</tt></td>
<td><tt>69</tt></td>
<td><tt>76</tt></td>
<td style="background:#dbd;"><tt>65</tt></td>
<td style="background:#dbd;"><tt>00</tt></td>
</tr>
</table>

> B's value has now been inadvertently replaced by a number formed from part of the character string. In this example "e" followed by a zero byte would become 25856.

> Writing data past the end of allocated memory can sometimes be detected by the operating system to generate a segmentation fault error that terminates the process.

> To prevent the buffer overflow from happening in this example, the call to strcpy could be replaced with strncpy, which takes the maximum capacity of A as an additional parameter and ensures that no more than this amount of data is written to A:

```c
strncpy(A, "excessive", sizeof(A));
```

### Run-time checking: StackGuard

> Modern operating systems use a variety of techniques to combat malicious buffer overflows, 
notably by randomizing the layout of memory, 
or deliberately leaving space between buffers and looking for actions 
that write into those areas ("canaries").
[Wikipedia](https://en.wikipedia.org/wiki/Buffer_overflow)

> Typically, buffer overflow protection modifies the organization of 
stack-allocated data so it includes a canary value that, when destroyed 
by a stack buffer overflow, shows that a buffer preceding it in memory 
has been overflowed. By verifying the canary value, execution of the affected 
program can be terminated, preventing it from misbehaving or from allowing an 
attacker to take control over it.
[Wikipedia](https://en.wikipedia.org/wiki/Buffer_overflow_protection)

### PaX

> PaX is a patch for the Linux kernel that implements least privilege 
protections for memory pages. The least-privilege approach allows 
computer programs to do only what they have to do in order to be able to 
execute properly, and nothing more. PaX was first released in 2000.

> PaX flags data memory as non-executable, program memory as non-writable 
and randomly arranges the program memory. This effectively prevents many 
security exploits, such as some kinds of buffer overflows. The former 
prevents direct code execution absolutely, while the latter makes so-called 
return-to-libc (ret2libc) attacks difficult to exploit, relying on luck 
to succeed, but doesn't prevent overwriting variables and pointers.
[Wikipedia](https://en.wikipedia.org/wiki/PaX)

### Problem: Lack of Diversity

Making the address layouts unpredictable between different machines.

### ASLR

> Address space layout randomization (ASLR) is a computer security technique 
involved in protection from buffer overflow attacks. In order to prevent an 
attacker from reliably jumping to, for example, a particular exploited function 
in memory, ASLR randomly arranges the address space positions of key data areas
of a process, including the base of the executable and the positions of the stack, 
heap and libraries.
[Wikipedia](https://en.wikipedia.org/wiki/Address_space_layout_randomization)

### Bugs

> **Goto Fail bug:** The Apple Computer, Inc. "goto fail" bug was a duplicated line of code which caused a public key certificate check to pass a test incorrectly. [Wikipedia](https://en.wikipedia.org/wiki/List_of_software_bugs)

> **Heartbleed bug:** Heartbleed, an OpenSSL vulnerability introduced in 2012 and disclosed in April 2014, removed confidentiality from affected services, causing among other things the shut down of the Canada Revenue Agency's public access to the online filing portion of its website[45] following the theft of social insurance numbers. [Wikipedia](https://en.wikipedia.org/wiki/List_of_software_bugs)

> **Shellshock bug:** Shellshock, also known as Bashdoor,[1] is a family of security bugs[2] in the widely used Unix Bash shell, the first of which was disclosed on 24 September 2014.
The first bug causes Bash to unintentionally execute commands when the commands are concatenated to the end of function definitions stored in the values of environment variables.
[Wikipedia](https://en.wikipedia.org/wiki/Shellshock_(software_bug))


## Threats: Hardware

> **Row Hammer bug:** Row hammer (also written as rowhammer) is an unintended side effect in dynamic random-access memory (DRAM) that causes memory cells to leak their charges and interact electrically between themselves, possibly altering the contents of nearby memory rows that were not addressed in the original memory access. This circumvention of the isolation between DRAM memory cells results from the high cell density in modern DRAM, and can be triggered by specially crafted memory access patterns that rapidly activate the same memory rows numerous times.
[Wikipedia](https://en.wikipedia.org/wiki/Row_hammer)

[Drammer: Deterministic Rowhammer Attacks on Mobile Platforms](https://vvdveen.com/publications/drammer.pdf)


---

# Vocabulary

* 15.1 The Security Problem
	* Secure
	* Intruder, cracker
	* Threat
	* Attack
	* Breach of confidentiality
	* Breach of integrity
	* Breach of availability
	* Theft of service
	* Denial of service DOS
	* Masquerading
	* Authentication
	* Replay attack
	* Message modification
	* Man-in-the-middle attack
	* Session hijacking
	* Security measures:
		* Physical
		* Human
			* Social Engineering
			* Phishing
			* Dumpster Diving
		* Operating system
		* Network
	
* 15.2 Program Threats
	* Back-door
	* Trojan Horse
		* Spyware
		* Covert channels
	* Trap door
		* Logic bomb
	* Stack and Buffere Overflow
		* Script kiddie
	* Viruses
		* Virus dropper
		* File (virus)
		* Boot (virus)
		* Macro (virus)
		* Source code (virus)
		* Polymorphic (virus)
		* Encrypted (virus)
		* Stealth (virus)
		* Tunneling (virus)
		* Multipartite (virus)
		* Armored (virus)
		* Keystroke logger
		* Monoculture

* 15.3 System and Network Threats
	* Secure by default
	* Attack surface
	* Worms
		* Spawn
		* Grappling hook / bootstrap / vector
	* Port scanning
		* Zombie systems
	* Denial of service
		* Distributed denial of service DDOS

* 15.4 Cryptography as a Security Tool
	* Cryptography
	* Keys
	* Encryption
		* Symmetric encryption algorithm
			* Data-encryption standard DESs
			* Block cipher
			* Triple DES
			* Advanced encryption standard AES
			* RC4
			* Stream cipher
			* Keystream
		* Asymmetric encryption
			* Public-key encryption
			* RSA
			* Public key, private key
		* Authentication
			* Hash function
			* Message digest / hash value
			* Message-authentication code MAC
			* Digital-signature algorithm
			* Nonrepudiation
		* Key distribution
			* Out-of-band
			* Key ring
			* Digital certificate
			* Certificate authorities
		* Implementation of cryptography
			* Layers
			* IPSec
			* Internet Key Exchange IKE
			* Virtual Private Networks VPNs
		* SSL

* 15.5 User Authentication
	* Passwords
	* Password vulnerabilities
		* Shoulder surfing
		* Sniff
	* Securing passwords
	* One-time passwords
		* Paired passwords
		* Personal identification number PIN
		* Two-factor authentication
		* Code book
		* One-time pad
	* Biometrics

* 15.6 Implementing Security Defenses
	* Defense in depth
	* Security policy
		* Living document
	* Vulnerability assessment
		* Risk assessment
		* Penetration test
		* Security through obscurity
	* Intrusion detection
		* Honeypot
		* Intrusion-detection systems IDSs
		* Intrusion-prevention systems IDPs
		* Signature-based detection
		* Anomaly detection
		* Zero-day attacks
		* False positives
		* False negatives
		* Bayes' theorem
	* Virus protection
		* Disinfecting
		* Sandbox
		* Safe computing
	* Auditing, accounting, and logging

* 15.7 Firewalling to Protect Systems
	* Security domains
	* Demilitarized zone DMZ
	* Tunnel
	* Spoofing
	* Personal firewall
	* Application proxy firewall
	* XML firewall
	* System-call firewalls

* 15.8 Computer-Security Classification
	* Trusted computer base TCB
	* Certification
	* Accredited








