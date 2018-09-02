# HomeBrew Install with spectific version

refer to [article](https://www.jianshu.com/p/aadb54eac0a8)

```Bash
git clone git@github.com:Homebrew/homebrew-core.git
git log ./Formula/boost.rb | less
# 'boost: update 1.66.0 bottle.'
git checkout 76a83d7
brew install ./Formula/boost.rb
```

backup formula as local file:

```Bash
cp -a ./Formula/boost.rb ./LocalFormula/boost-1.66.rb
brew install ./LocalFormula/boost-1.66.rb
```
