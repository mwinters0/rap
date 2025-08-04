# rap
Streaming (unbuffered) line wraps

## Usage
```bash
# with ollama
curl -s --show-error --no-buffer http://localhost:11434/api/generate \
    -d '{"model": $model, "prompt": $prompt}' \
    | jq --raw-output0 --unbuffered '.response' \
    | RAPWIDTH=80 ./rap
```

## Why
I wanted the results to stream to my terminal like in the web GUI (partially so I can make sure they're actually coming), but wide text is hard to read.  I basically wanted `cat` but unbuffered and with configurable line wrapping.

Surprisingly, this doesn't exist.  `fold`, `fmt`, and `pr` all do various levels of internal buffering that you can't disable externally with stdio tricks like `stdbuf`.  Presumably this is possible by abusing some other utility like sed, but it seemed easier to code myself.

I don't expect anyone else to want such a thing, so no arg parsing or file loading here - only RAPWIDTH and stdin.  Oh, and no UTF awareness.  It shouldn't break any output, but it will think that multibyte characters are wider than they really are and thus wrap early.

## Why C?
I had some Steely Dan playing on vinyl and just went with it.
