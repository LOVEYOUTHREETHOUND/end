#include <Wire.h>  // Wire 库，用于IC通信
int ADXL345 = 0x53; //ADXL345 三轴重力加速度传感器 I2C 地址
float X_out, Y_out, Z_out;  // 输出数据

int C[] = {2,7,A3,5,13,A2,12,A0}; //行
int R[] = {6,11,10,3,A1,4,8,9};   //列
 
unsigned char biglove[8][8] =       //大“心型”的数据
{
  0,0,0,0,0,0,0,0,
  0,1,1,0,0,1,1,0,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  0,1,1,1,1,1,1,0,
  0,0,1,1,1,1,0,0,
  0,0,0,1,1,0,0,0,
};
 
unsigned char smalllove[8][8] =      //小“心型”的数据
{
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,1,0,0,1,0,0,
  0,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,0,
  0,0,1,1,1,1,0,0,
  0,0,0,1,1,0,0,0,
  0,0,0,0,0,0,0,0,
};
unsigned char left[8][8] =       //←的数据
{
  0,0,0,1,1,0,0,0,
  0,0,1,1,0,0,0,0,
  0,1,1,0,0,0,0,0,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  0,1,1,0,0,0,0,0,
  0,0,1,1,0,0,0,0,
  0,0,0,1,1,0,0,0,
}; 
unsigned char right[8][8] =       //→的数据
{
  0,0,0,1,1,0,0,0,
  0,0,0,0,1,1,0,0,
  0,0,0,0,0,1,1,0,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  0,0,0,0,0,1,1,0,
  0,0,0,0,1,1,0,0,
  0,0,0,1,1,0,0,0,
}; 
unsigned char forward[8][8] =       //↑的数据
{
  0,0,0,1,1,0,0,0,
  0,0,1,1,1,1,0,0,
  0,1,1,1,1,1,1,0,
  1,1,0,1,1,0,1,1,
  1,0,0,1,1,0,0,1,
  0,0,0,1,1,0,0,0,
  0,0,0,1,1,0,0,0,
  0,0,0,1,1,0,0,0,
}; 
unsigned char backward[8][8] =       //↓的数据
{
  0,0,0,1,1,0,0,0,
  0,0,0,1,1,0,0,0,
  0,0,0,1,1,0,0,0,
  1,0,0,1,1,0,0,1,
  1,1,0,1,1,0,1,1,
  0,1,1,1,1,1,1,0,
  0,0,1,1,1,1,0,0,
  0,0,0,1,1,0,0,0,
}; 
void setup()
{
  for(int i = 0;i<8;i++)
  {
    pinMode(R[i],OUTPUT);
    pinMode(C[i],OUTPUT);
  }
  Serial.begin(9600); // 启动串行通信，将结果打印到串口监视器
  Wire.begin(); //初始化连接库
  // 建立 ADXL345 测量模式
  Wire.beginTransmission(ADXL345); // 开始与设备通信
  Wire.write(0x2D); // 访问电源CTL寄存器0x2D
  // 使测量
  Wire.write(8); // (8dec -> 0000 1000 binary)  D3 高测量启用
  Wire.endTransmission();
  delay(10);
}
 
void loop()
{
  
  if(Y_out>0.3){                  //如果y轴偏转角>0.3，则LED点阵显示左箭头
    for(int i = 0 ; i < 100 ; i++)        
  {
    Display(left);                   
  }
}  
  else if(Y_out<-0.3){            //如果y轴偏转角<-0.3，则LED点阵显示右箭头
    for(int i = 0 ; i < 100 ; i++)
    {   
      Display(right);                 
    }         
  }
  else if(X_out>0.3){             //如果x轴偏转角>0.3，则LED点阵显示向下箭头
    for(int i = 0 ; i < 100 ; i++)
    {   
      Display(backward);                 
    }         
  }
  else if(X_out<-0.3){            //如果x轴偏转角<-0.3,则LEDD点阵显示上箭头
    for(int i = 0 ; i < 100 ; i++)
    {   
      Display(forward);                 
    }         
  }
  else{                           //若偏转角范围大于-0.3小于0.3，则大心形与小心形交替闪烁
    for(int i = 0 ; i < 100 ; i++)
    {   
      Display(biglove);                 
    }         
    for(int i = 0 ; i < 100 ; i++)
    {   
     Display(smalllove);                 
    }         
  }
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // 从寄存器 0x32 (ACCEL_XOUT_H)开始
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // 共读取6个寄存器，每个轴的值储存在两个寄存器中
  X_out = ( Wire.read()| Wire.read() << 8); // X轴值
  X_out = X_out/256; 
  Y_out = ( Wire.read()| Wire.read() << 8); // Y轴值
  Y_out = Y_out/256;
  Z_out = ( Wire.read()| Wire.read() << 8); // Z轴值
  Z_out = Z_out/256;
  Serial.print("Xa= ");
  Serial.print(X_out);
  Serial.print("   Ya= ");
  Serial.print(Y_out);
  Serial.print("   Za= ");
  Serial.println(Z_out); 
}

 
void Display(unsigned char dat[8][8])   //显示函数
{
  for(int c = 0; c<8;c++)
  {
    digitalWrite(C[c],LOW);//选通第c列
 
    //循环
    for(int r = 0;r<8;r++)
    {
      digitalWrite(R[r],dat[r][c]);
    }
    Clear();  //清空显示去除余晖
  }
}
 
void Clear()                          //清空显示
{
  for(int i = 0;i<8;i++)
  {
    digitalWrite(R[i],LOW);
    digitalWrite(C[i],HIGH);
  }
}
