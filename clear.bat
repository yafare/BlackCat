del /Q /s /f *.suo
del /Q /s *.sdf

FOR /R . %%d IN (.) DO rd /s /q "%%d\logs" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\@bin" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\@lib" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\@obj" 2>nul
