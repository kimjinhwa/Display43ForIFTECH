 ## 사용방법
 - ui_label과 같은 컴포넌트들은 `ui_init()`를 해주면 적용된다. 
 - upsLog 클래스는 생성시에 Language가 결정된다.    
 따라서 시스템을 재 가동하거나 클래스가 재 로드되어야 한다.
 ### extract multi language.
 ```sh
 lv_i18n extract -s 'src/**/*.+(c|cpp|h|hpp)' -t 'translations/*.yml'
`````` 
 ### Compile multi language.
```sh


lv_i18n compile -t 'translations/*.yml' -o 'src/lv_i18n'
or
lv_i18n compile -t 'src/resource/*.yml' -o 'lib/lv_i18n/src/'

lv_i18n compile -t 'C:/DevWork/99.SquireLineWave/IFTECH_UI/UPS1P1P_V001/export/SquareLine_Project/libraries/ui/translations/*.yml' -o 'C:\DevWork\4.IFTechWork\1.UPS1P1P\Display4.3\lib\lv_i18n\src'
``` 

