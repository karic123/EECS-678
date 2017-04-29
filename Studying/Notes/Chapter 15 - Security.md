# Chapter 15: Security

Rachel's summary notes

* [Official book slides - Chapter 15](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch15.ppt)

---

# Slide notes

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
to the computer's resources. The famed Morris worm used this as one of its attack techniques.
[Wikipedia](https://en.wikipedia.org/wiki/Buffer_overflow)



### Run-time checking: StackGuard

### PaX

### Problem: Lack of Diversity

### ASLR

### Goto Fail bug

### Heartbleed bug

### Shellshock bug

## Threats: Hardware

### Drammer

## Summary

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








