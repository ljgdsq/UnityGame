set WORKSPACE=.\Luban

set GEN_CLIENT=%WORKSPACE%\Tools\Luban\Luban.dll
set CONF_ROOT=%WORKSPACE%\DataTables

dotnet %GEN_CLIENT% ^
    -t client ^
    -c cs-simple-json ^
    -d json  ^
    --conf %CONF_ROOT%\luban.conf ^
    -x outputCodeDir=Assets/Gen ^
    -x outputDataDir=..\GenerateDatas\json 
    @REM ^
    @REM -x l10n.provider=default ^
    @REM -x l10n.textFile.path=*@%WORKSPACE%\DataTables\Datas\l10n\texts.json ^
    @REM -x l10n.textFile.keyFieldName=key

pause