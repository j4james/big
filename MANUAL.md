BIG(1)
------

#### NAME ####

**big** - Befunge integer generator

#### SYNOPSIS ####

    big [options...] <number>
    big [options...] <number>..<number>

#### DESCRIPTION ####

The Befunge Integer Generator attempts to generate the shortest possible
Befunge expression that will evaluate to a given number.

It can accept a list of individual numbers (e.g. `5 10 15`) or a number range
(e.g. `10..20`). The input is interpreted as decimal by default, but can be
marked as hexadecimal with an `h` suffix (e.g. `7Fh`).

The following options are also available:

| Option        | Description                                                 |
|---------------|-------------------------------------------------------------|
| -h, --help    | Display this help                                           |
| -f, --fast    | Prefer faster results over shorter string literals          |
| -n, --numeric | Only allow numeric expressions (no strings)                 |
| -s, --safe    | Disallow unsafe characters: SPACE, ';' and '@'              |
| -a, --all     | Show all variants matching the minimal length               |
|     --93      | Befunge-93 compatible expressions (default)                 |
|     --98      | Befunge-98 compatible expressions                           |

By default the generator prefers expressions with shorter string literals,
since those can more easily be split when having to change direction in a
piece of Befunge code. If the **fast** option is set, though, the generator
will just return the first minimal-length result it finds.

If the **numeric** option is set, the generator will avoid strings literals
altogether, which can be useful in certain restricted-source scenarios.

The **safe** option will prevent the use of a few characters that are known
to have portability problems. This include the SPACE character (which some
interpreters drop), the `;` character (which can be misinterpreted as a jump
operation), and the `@` character (which can force some implementation to
terminate).

If the **all** option is set, the generator will return every minimal-length
expression it can find for a particular number.

By default, the generated expression are compatible with Befunge-93, but if
the **98** option is set, they may use language extensions specific to the
Befunge-98 standard. This includes hex literals, as well as the one-shot
string mode. Technically these extensions are also supported by Befunge-96
and -97, but those versions of the language are rarely used anymore.

#### LIMITATIONS ####

In most cases the generated expressions should be optimal, but that can't
always be guaranteed. The **numeric** mode in particular is known to produce
results that are less than optimal for some values.

Even when optimal, the **all** option will not be guaranteed to return every
possible expression that matches the best length. There are a limited set of
strategies used by the generator, so those are obviously the only variants
that will be returned.

#### AUTHOR ####

Written by James Holderness.

#### SEE ALSO ####

[BefunRep](https://github.com/Mikescher/BefunRep),
[Fungify](https://deewiant.iki.fi/projects/fungify),
[Befour](https://codegolf.stackexchange.com/a/78561)
