using CliWrap.Buffered;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xunit;
using Xunit.Abstractions;

namespace CliWrap.Tests;
public class MyTestSpecs
{



    ITestOutputHelper _logger; 

    public MyTestSpecs(ITestOutputHelper outputHelper)
    {
        _logger = outputHelper;
    }
    [Fact]
    public void SimpleLog()
    {
        _logger.WriteLine("SimpleCommand hello");
    }


    [Fact]
    public async void SimpleWrapCommand()
    {
        var cmd=Cli.Wrap("cmd").WithArguments("dir");
        var result=await cmd.ExecuteBufferedAsync();
        _logger.WriteLine(result.StandardOutput);
    }
}
