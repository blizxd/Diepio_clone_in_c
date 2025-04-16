# This is the devlog of my game

| Task                                   | Date       |
| -------------------------------------- | ---------- |
| Initialize SDL                         | 2024-11-12 |
| Draw a screen                          | 2024-11-12 |
| Draw a rectangle                       | 2024-11-12 |
| Draw player sprite                     | 2024-11-13 |
| Make player rotate to cursor direction | 2024-11-13 |
| Setup SDL_image library                | 2024-11-13 |
| Use png texture                        | 2024-11-14 |
| Restructure project                    | 2024-11-17 |
| Improved controls                      | 2024-11-18 |
| Added scrolling                        | 2024-11-19 |
| Added level borders                    | 2024-11-19 |
| Added player obstacle collision        | 2024-11-19 |
| Improved obstacle spawning             | 2024-11-20 |
| Implemented resource manager           | 2024-11-20 |
| Added bullets                          | 2024-11-20 |
| Added knockback effect                 | 2024-11-20 |
| Show score                             | 2024-11-25 |
| Show game over screen                  | 2024-11-25 |
| Save current game data in a file       | 2024-11-26 |

## TODO:

- Make enemies

## Useful snippets :

### FPS counter

```c
clock_t tickCounter = clock();
...
printf("FPS : %.4lf \n", 1 / ((double)(clock() - tickCounter) / CLOCKS_PER_SEC));
tickCounter = clock();
```

## Useful resources :

- https://thenumb.at/cpp-course/sdl2/02/02.html
- https://lazyfoo.net/tutorials/SDL/index.php
- https://www.youtube.com/playlist?list=PLvv0ScY6vfd-p1gSnbQhY7vMe2rng0IL0
- https://www.youtube.com/@CarlBirch
