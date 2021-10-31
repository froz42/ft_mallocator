<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/tmatis/ft_mallocator">
    <img src="ressources/logo.png" alt="Logo" width="232" height="41">
  </a>

  <p align="center">
    Test your allocs protections and leaks !
    <a href="https://github.com/tmatis/ft_mallocator/issues">Report Bug</a>
    ·
    <a href="https://github.com/tmatis/ft_mallocator/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-tool">About The Tool</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#quickstart">Quickstart</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- About The Tool -->
## About The Tool

![screenshot](/ressources/screenshot.png?raw=true)

This tool allows you to test if **every** allocs in your project are protected.
It also checks if an alloc fail: if you *free* every allocations.

It also check leaks !

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

Here, you will see how to setup this tool in a example project.

### Prerequisites

This is working out of the box on 42's VM

If your are not on the VM you will need those installed on your system.

**Your CC must be clang**

* Linux
* clang
* llvm

### Quickstart


1. Clone the repo inside your project
   ```sh
   git clone https://github.com/tmatis/ft_mallocator.git
   ```
2. Cd in directory
   ```sh
   cd ./ft_mallocator
   ```
3. Execute script
   ```sh
   bash test.sh
   ```
4. Follow script's instructions...

### Demo video
https://user-images.githubusercontent.com/54767855/139539158-52ab77dd-8dec-4678-9ff0-cd1dee8fe68f.mp4

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Here we have an example of not well protected code:
```c
char *malloc_function(void)
{
	return (malloc(1000));
}

int main(void)
{
	void *ptr = malloc_function();
	if (ptr == NULL)
		return (1);
	free(ptr);

	void *array[10];

	for (int i = 0; i < 10; i++)
	{
		array[i] = malloc(1000);
		if (array[i] == NULL)
		{
			printf("it fail %i\n", i);
			return (0);
		}
		memset(array[i], 0, 1000);
	}
	
	for (int i = 0; i < 10; i++)
		free(array[i]);

	ptr = malloc_function();
	memset(ptr, 0, 1000);
	free(ptr);
}
```

this exemples have many problems, let's run mallocator on it...
![screenshot](/ressources/screenshot.png?raw=true)

The first allocation is well protected nothing to say.
The second alloction if protected but if a malloc crash not everythings is free. (At the second iteration of the for loop)
The third allocation is not protected at all, there is no null check.

Let's see a well protected example..

```c
char *malloc_function(void)
{
	return (malloc(1000));
}

int main(void)
{
	void *ptr = malloc_function();
	if (ptr == NULL)
		return (1);
	free(ptr);

	void *array[10];

	int i = 0;
	for (i = 0; i < 10; i++)
	{
		array[i] = malloc(1000);
		if (array[i] == NULL)
		{
			printf("it fail %i\n", i);
			break ;
		}
		memset(array[i], 0, 1000);
	}
	for (int j = 0; j < i; j++)
		free(array[j]);

	ptr = malloc_function();
	if (!ptr)
		return (1);
	memset(ptr, 0, 1000);
	free(ptr);
}
```

Then we run mallocator on this code ...

![screenshot](/ressources/well_protected.png?raw=true)

This code is protected.

You can see how it is working behind the scene ![here](/how_it_work.md).

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

* **Théo Matis** tmatis@student.42.fr

[![tmatis's 42 stats](https://badge42.herokuapp.com/api/stats/tmatis)](https://github.com/JaeSeoKim/badge42)

<p align="right">(<a href="#top">back to top</a>)</p>
