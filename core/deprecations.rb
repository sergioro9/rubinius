module Rubinius
  DEPRECATIONS = {
    # "Description" => "Alternative"
    "Ruby 2.1/2.2 features that are incompatible with Ruby 2.3 are deprecated." =>
      "Use Ruby 2.3 features if they are available.",
    "Rubinius::ConstantScope is deprecated." =>
      "Use Rubinius::LexicalScope instead.",
    "Rubinius::Executable#primitive is deprecated." =>
      "Use Rubinius::Executable#tags instead.",
    "Rubinius::RUNTIME_PATH is deprecated." =>
      "Use Rubinius::CODEDB_PATH instead.",
    "Rubinius::CORE_PATH is deprecated." =>
      "Use Rubinius::CODEDB_PATH/source/core instead, or retrieve the path from the CompiledCode#file attribute",
    "Non-UTF-8 encodings for String objects are deprecated." =>
      "Use String objects encoded as UTF-8.",
    "Encoding objects other than UTF-8 are deprecated." =>
      "Use ByteString (when available) to passed text data with non-UTF-8 encodings through a program. Use transcoding facilities at IO boundaries to read from or write to non-UTF-8 encodings. Use File.transcode to transcode a file from one encoding to another.",
    "Mutating a String object is deprecated." =>
      "Use ByteString for binary data or MutableString for text (when available).",
    "Insertion ordering of Hash is deprecated." =>
      "Use OrderedHash when insertion ordering is desired.",
    "OpenSSL is deprecated." =>
      "Use the core cryptography and TLS libraries when available.",
    "The Ruby stdlib is deprecated." =>
      "Use core libraries (eg Socket) or alternative gems when available.",
    "Rubygems and Bundler are deprecated." =>
      "Use the IPFS library facility when available.",
    "Use of the identifier 'type' at script scope is deprecated." =>
      "Use an identifier not named 'type' at script scope.",
    "Use of the identifier 'import' at script, class/module, method, block, or function scope is deprecated." =>
      "Use an identifier not named 'import' at script, class/module, method, block, or function scope.",
    "Defining a new method without removing an existing method definition for that name is deprecated." =>
      "Defining a new method with the same name as an existing method will define multiple methods. Use `remove_method` followed by `def` to replace a method.",
  }
end
