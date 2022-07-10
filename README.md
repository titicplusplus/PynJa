# PynJa

PynJa is a templating engine written in C++ that use python3 syntax with its interpreter to render file.

# Install

First, you need to install python3, libpython3 and nlohmann/json and then you can run:

```
git clone https://github.com/titicplusplus/PynJa.git
cd PynJa
mkdir build && cd build
cmake ..
make
sudo make install
```

# How to use it

## The current syntax

=> To write python code, you have to write:
```
$$$
# python code Here
$$$
```

=> If you want to execute only one line, you can use this syntax:
```
$#python code here
```

=> If you want to include a file
```
$filename
```

=> if you want to print a variable, you have to use this syntax:
```
{variable_name}
```

And that's it !
You also have to know how to write Python code.

### Full Example:

Here is an example of file that can be run by the PynJa (`Example/index.thi`):

```python
$$$
debug = True
a = 0

def test(a, b):
	return a*b

if debug:
	a = 3
$$$
$$ ui/menu.thi

The list is : \
$if debug == True:
	$for i in range(len(lst)):
		$if i != len(lst) - 1:
			{lst[i]} - \
		$else:
			{lst[i]}\
		$endif
	$endfor
$endif #Very important
	
You can do anything

$$ ui/menu.thi

Test(a, 2) => {test(a, 2)}
An example of dict : {ex_dict}

$from datetime import datetime #You can import module
Current time = {datetime.now().strftime('%H:%M:%S')}
```

And the result might be (depend of the current time):

```
Hey! I am a menu
Hi! I'm the sub-menu

The list is : a - 8 - 65 - True	
You can do anything

Hey! I am a menu
Hi! I'm the sub-menu

Test(a, 2) => 6
An example of dict : {'k': 4, 'r': 3}

Current time = 11:45:33
```

## How to use in a C++ software

### The C++ code:

It's really easy to use PynJa.
Here is an example:

```
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

#include <TitiCplusplus/pynja.hpp>

int main() {
	TitiCplusplus::PynJa::init(); /// First, you have to initialise the library, very important !!!

	/// Then import data
	nlohmann::json dataUser = {
		{"First", "Hi! EveryBody"},
		{"ex_dict", {
			{"r", 3},
			{"k", 4},
		}},
		{"lst", {"a", 8, 65, true}},
	};

	/**
	or if you want to get data from a json file
	{
		std::ifstream flux {"data.json"};
		flux >> dataUser;
		flux.close();
	}
	**/

	/// Create a PynJa object
	TitiCplusplus::PynJa reader;

	/// reader.getResult with open the file and render it
	/// "" => is the directory (ex: "files/render/"
	/// "index.thi" => the file
	/// dataUser => the data to render the file
	std::cout << reader.getResult("", "index.thi", dataUser);

	TitiCplusplus::PynJa::end();

	return 0;
}
```

### How to run it:

You can compile by run in the `Example` directory:
(you have to find your python version)
```
g++ main.cpp -o test -lPynJa -I/usr/include/python3.8
./test
```



