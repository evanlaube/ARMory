# Contributing to ARMORY

Thank you for considering contributing to ARMory!

Writing low-level embedded systems code takes a lot of effort, so really - I
appreciate you even thinking about jumping in.

Please note that even the smallest of contributions are not only welcome, but
actively create a better HAL. 

>So yes, **please** open that pull request that fixes all the typos I've made 
(there's probably a lot)

## How To Contribute
To get started:

1. **Fork the repository**
   - Click the `Fork` button in the top right corner of the repo page.

2. **Clone your fork**
    ```bash
    git clone https://github.com/YOUR_USERNAME/ARMory.git
    cd ARMory
    ```
3. **Create a new branch for your change**
    ```bash
    git checkout -b feature/your-feature-name
    ```

4. **Make your changes**
   - Follow the **Code Guidelines**.

5. **Commit with a clear message**
   - See `Commit Message Style`.

6. **Push your branch**
    ```bash
    git push origin feature/your-feature-name
    ```

7. Open a Pull Request
   - Go to your fork on GitHub and click on `Compare and Pull Request`.
   - Describe what you changed and why.

> Don't worry about perfection. It's better to open a PR and start a conversation
than spend hours trying to get it perfect

8. **Success!**
   - Once your PR is merged in, you have officially made a contribution! Celebrate!

## Code Guidelines

To keep the ARMory codebase clean and consistent, please follow these general
guidelines:

### Formatting and Style
- **Use 4 spaces for indentation.** No tabs allowed :(

- **Curly braces MUST go on their own lines** (controversial, sorry)
    ```C
    // DO THIS:
    if (condition) {
        doSomething();
    }

    // NEVER THIS:
    if (condition)
    {
        doSomethingIncorrectly();
    }
    ```

- **Avoid "one-liner" if statements and for/while loops**
    - Adding a couple extra brackets is more readable. It doesn't hurt
    anybody, and it makes the code easier to append to
    ```C
    // DON'T DO THIS:
    if(condition) doSomethingIncorrectly();

    // DO THIS INSTEAD
    if(condition) {
        doSomethingCorrectly();
    }
    ```

- Place `const` on the left (`const Type *ptr`, not `Type const *ptr`)

- **Keep lines ~80-100 characters max, especially for register definitions and macros**

### File Structure
- Header files should include guards (`#ifndef`, not `#pragma once`)
- Group related functions by module (e.g. `gpio.c`, `i2c.c`)
Keep hardware register definitions in headers, and implementation in `.c` files

### Naming conventions
- Functions must use `snakeCase` prefixed with the module
  - `i2cInit`, `gpioWrite`, `pwmInitAll`, etc.

- Enums and typedefs use `PascalCase`
  - `AlternateFunction`, `Pin`, `I2CResult`, etc.

- Macro names and register flags are capitalized and use underscores 
  - `GPIOA_BASE`, `A15`, `ADC1_SR_OFFSET`

### Comment Style
- To keep lines short, prefer **full-line comments**
  - inline comments acceptable
in some cases, as long as they do not make the line exceed 100+ characters

    ```C
    // Example:
    
    // Function to fill an array with alternating 1's and 0's
    int func(int length, int *arr) {
        // Iterate `length` times
        for(int i = 0; i < length; i++) {
            // Insert i % 2 to the array at the i'th position
            arr[i] = i % 2;
        }
    }
    
    // These comments might be a little self explanatory, but that's okay!
    // Too many comments are always better than none at all.
    ```

- Follow Doxygen-styled comments for function prototypes in header files
  - Block must start with `/**` (Notice the extra `*`)
  - `@brief` must be included at a minimum, along with all `@param`, but other fields are optional
    ```C
    
    /**
     * @brief Set the mode of a GPIO pin.
     * 
     * Configures a given GPIO pin to operate as an input, output, analog input,
     * or alternate function.
     *
     * @param pin The GPIO pin to configure.
     * @param mode The desired mode to set the pin to.
     * 
     * @note The GPIO port clock must be enabled before calling this function.
     */
    void gpioPinMode(Pin pin, PinMode mode);
    ```

- Use block comments in code for sections containing complicated algorithms.

## Commit Message Style
- Use a short, descriptive commit title in the format:
```
[module] Short description of the change
```

**Examples:**
```
[gpio] Add support for setting pin pull mode
[i2c] Fix timeout bug when sending address
```

- For larger changes, you can add a blank line and a longer explanation in the body
```
[i2c] Improve error handling for NACK responses

This fixes a case where a missing ACK caused the driver to hang indefinitely
instead of returning a proper error code
```

- **Keep the title short!** - Try to keep it under ~50 characters if possible

> Tip: Think of a commit message as completing the following sentence: \
"When applied, this commit will [your message]."

## Testing
As of now, and for the foreseeable future, no official tests are active for 
ARMory. Therefore, the `examples/` directory acts as the best way to test new
features. 

Please create a new example (or modify an existing one) that uses the ARMory feature
you're creating or modifying. Preferably use simple, accessible hardware (LEDs,
buttons, potentiometers, etc)
> Optional: Add a README to your example for wiring instructions

**Please, please** test your code on actual hardware. Just because it compiles does
not mean it works. Including a working example with your code changes
can make it easy for others to see exactly how your library functions should be
created.

> Note: Very small changes (like typos) do not necessarily need to be tested before
being committed.
