xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 74;
 -5.50220;-2.83057;3.02951;,
 -2.40120;-2.41196;2.58947;,
 -2.40119;0.05148;3.66207;,
 -5.50221;0.05150;4.28438;,
 -2.40119;2.51498;2.58947;,
 -5.50220;2.93352;3.02951;,
 -2.40120;3.53536;0.00000;,
 -5.50221;4.12737;0.00000;,
 -2.40119;2.51498;-2.58947;,
 -5.50220;2.93352;-3.02951;,
 -2.40119;0.05148;-3.66207;,
 -5.50221;0.05150;-4.28438;,
 -2.40120;-2.41196;-2.58947;,
 -5.50220;-2.83057;-3.02951;,
 -2.40119;-3.43236;0.00000;,
 -5.50221;-4.02436;0.00000;,
 -9.95701;0.05148;4.80461;,
 -9.95701;0.05148;0.00000;,
 -9.95700;-3.18055;3.39737;,
 -9.95700;3.28353;3.39737;,
 -9.95701;4.62226;0.00000;,
 -9.95700;3.28353;-3.39737;,
 -9.95701;0.05148;-4.80461;,
 -9.95700;-3.18055;-3.39737;,
 -9.94949;-4.51930;0.00000;,
 2.05925;-1.06286;1.17136;,
 2.47875;0.05148;0.00000;,
 2.05926;0.05151;1.65655;,
 2.05925;1.16586;1.17136;,
 2.05925;1.62739;0.00000;,
 2.05925;1.16586;-1.17136;,
 2.05926;0.05151;-1.65655;,
 2.05925;-1.06286;-1.17136;,
 2.05925;-1.52442;-0.00000;,
 -8.69776;-3.21375;3.43229;,
 -8.69777;0.05148;4.85400;,
 -8.69776;3.31675;3.43229;,
 -8.69777;4.66924;0.00000;,
 -8.69776;3.31675;-3.43229;,
 -8.69777;0.05148;-4.85400;,
 -8.69776;-3.21375;-3.43229;,
 -8.69777;-4.56626;0.00000;,
 0.91892;-2.56693;-0.00000;,
 0.91893;-1.80001;1.94624;,
 0.91892;-1.80003;-1.94624;,
 0.91892;0.05148;-2.75240;,
 0.91892;1.90300;-1.94624;,
 0.91892;2.66994;0.00000;,
 0.91892;1.90300;1.94624;,
 0.91892;0.05148;2.75240;,
 -12.57832;-3.46201;3.69326;,
 -12.57833;0.05151;5.22305;,
 -12.57832;3.56501;3.69326;,
 -12.57832;5.02033;0.00000;,
 -12.57832;3.56501;-3.69326;,
 -12.57833;0.05151;-5.22305;,
 -12.57832;-3.46201;-3.69326;,
 -12.57833;-4.91736;0.00000;,
 -12.57834;-3.25724;3.47800;,
 -12.57833;-4.62775;0.00000;,
 -12.57834;-3.25724;-3.47800;,
 -12.57834;0.05151;-4.91863;,
 -12.57833;3.36023;-3.47800;,
 -12.57833;4.73074;0.00000;,
 -12.57833;3.36023;3.47800;,
 -12.57834;0.05151;4.91863;,
 -12.57834;0.05151;4.91863;,
 -12.57834;-3.25724;3.47800;,
 -12.57833;3.36023;3.47800;,
 -12.57833;4.73074;0.00000;,
 -12.57833;3.36023;-3.47800;,
 -12.57834;0.05151;-4.91863;,
 -12.57834;-3.25724;-3.47800;,
 -12.57833;-4.62775;0.00000;;
 
 72;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,1,0;,
 3;16,17,18;,
 3;19,17,16;,
 3;20,17,19;,
 3;21,17,20;,
 3;22,17,21;,
 3;23,17,22;,
 3;24,17,23;,
 3;18,17,24;,
 3;25,26,27;,
 3;27,26,28;,
 3;28,26,29;,
 3;29,26,30;,
 3;30,26,31;,
 3;31,26,32;,
 3;32,26,33;,
 3;33,26,25;,
 4;34,0,3,35;,
 4;35,3,5,36;,
 4;36,5,7,37;,
 4;37,7,9,38;,
 4;38,9,11,39;,
 4;39,11,13,40;,
 4;40,13,15,41;,
 4;41,15,0,34;,
 4;14,42,43,1;,
 4;12,44,42,14;,
 4;10,45,44,12;,
 4;8,46,45,10;,
 4;6,47,46,8;,
 4;4,48,47,6;,
 4;2,49,48,4;,
 4;1,43,49,2;,
 4;50,34,35,51;,
 4;51,35,36,52;,
 4;52,36,37,53;,
 4;53,37,38,54;,
 4;54,38,39,55;,
 4;55,39,40,56;,
 4;56,40,41,57;,
 4;57,41,34,50;,
 4;50,58,59,57;,
 4;57,59,60,56;,
 4;56,60,61,55;,
 4;55,61,62,54;,
 4;54,62,63,53;,
 4;53,63,64,52;,
 4;52,64,65,51;,
 4;51,65,58,50;,
 4;66,16,18,67;,
 4;68,19,16,66;,
 4;69,20,19,68;,
 4;70,21,20,69;,
 4;71,22,21,70;,
 4;72,23,22,71;,
 4;73,24,23,72;,
 4;67,18,24,73;,
 4;43,25,27,49;,
 4;49,27,28,48;,
 4;48,28,29,47;,
 4;47,29,30,46;,
 4;46,30,31,45;,
 4;45,31,32,44;,
 4;44,32,33,42;,
 4;42,33,25,43;;
 
 MeshMaterialList {
  1;
  72;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Chara_UV.png";
   }
  }
 }
 MeshNormals {
  73;
  0.092337;-0.716432;0.691519;,
  0.094677;-0.000001;0.995508;,
  0.092339;0.716434;0.691517;,
  0.090107;0.995932;0.000000;,
  0.092339;0.716434;-0.691517;,
  0.094677;-0.000001;-0.995508;,
  0.092337;-0.716432;-0.691519;,
  0.090108;-0.995932;-0.000000;,
  0.488855;-0.628613;0.604869;,
  0.497588;-0.000001;0.867414;,
  0.488855;0.628612;0.604870;,
  0.480460;0.877017;0.000000;,
  0.488855;0.628612;-0.604870;,
  0.497587;-0.000001;-0.867414;,
  0.488855;-0.628614;-0.604868;,
  0.480458;-0.877018;0.000001;,
  1.000000;-0.000000;-0.000000;,
  0.181651;-0.707642;0.682822;,
  0.186136;-0.000000;0.982524;,
  0.181649;0.707640;0.682824;,
  0.177365;0.984145;0.000000;,
  0.181649;0.707640;-0.682824;,
  0.186136;-0.000000;-0.982524;,
  0.181651;-0.707642;-0.682822;,
  0.177368;-0.984145;-0.000000;,
  0.225235;-0.701202;0.676450;,
  0.220028;-0.975494;0.000001;,
  0.225234;-0.701203;-0.676449;,
  0.230683;-0.000002;-0.973029;,
  0.225235;0.701200;-0.676452;,
  0.220027;0.975494;0.000000;,
  0.225235;0.701200;0.676452;,
  0.230683;-0.000002;0.973029;,
  0.131925;-0.713263;0.688369;,
  0.135231;-0.000000;0.990814;,
  0.131927;0.713262;0.688369;,
  0.128774;0.991674;0.000000;,
  0.131927;0.713262;-0.688369;,
  0.135231;-0.000000;-0.990814;,
  0.131925;-0.713263;-0.688369;,
  0.128773;-0.991674;-0.000000;,
  -1.000000;-0.000027;0.000029;,
  -1.000000;-0.000032;0.000000;,
  -1.000000;-0.000027;-0.000029;,
  -1.000000;-0.000003;-0.000038;,
  -1.000000;0.000025;-0.000022;,
  -1.000000;0.000034;0.000000;,
  -1.000000;0.000025;0.000022;,
  -1.000000;-0.000003;0.000038;,
  -1.000000;-0.000000;0.000000;,
  -0.999999;-0.000824;-0.000781;,
  -1.000000;-0.000411;0.000000;,
  -1.000000;0.000002;0.000001;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000002;-0.000001;,
  -0.999999;-0.000824;0.000781;,
  -0.999999;-0.001645;0.000000;,
  0.862545;-0.365474;0.349923;,
  0.867449;0.000001;0.497526;,
  0.862545;0.365476;0.349921;,
  0.857721;0.514116;0.000000;,
  0.862545;0.365476;-0.349921;,
  0.867449;0.000001;-0.497526;,
  0.862545;-0.365474;-0.349922;,
  0.857721;-0.514116;0.000001;,
  -0.043456;-0.000000;-0.999055;,
  -0.042341;0.718818;-0.693907;,
  -0.042378;-0.718832;-0.693891;,
  -0.041347;-0.999145;0.000000;,
  -0.042378;-0.718832;0.693891;,
  -0.043456;-0.000000;0.999055;,
  -0.042341;0.718818;0.693907;,
  -0.041280;0.999148;0.000000;;
  72;
  4;17,25,32,18;,
  4;18,32,31,19;,
  4;19,31,30,20;,
  4;20,30,29,21;,
  4;21,29,28,22;,
  4;22,28,27,23;,
  4;23,27,26,24;,
  4;24,26,25,17;,
  3;49,51,50;,
  3;52,51,49;,
  3;53,51,52;,
  3;54,51,53;,
  3;49,51,54;,
  3;55,51,49;,
  3;56,51,55;,
  3;50,51,56;,
  3;57,16,58;,
  3;58,16,59;,
  3;59,16,60;,
  3;60,16,61;,
  3;61,16,62;,
  3;62,16,63;,
  3;63,16,64;,
  3;64,16,57;,
  4;33,17,18,34;,
  4;34,18,19,35;,
  4;35,19,20,36;,
  4;36,20,21,37;,
  4;37,21,22,38;,
  4;38,22,23,39;,
  4;39,23,24,40;,
  4;40,24,17,33;,
  4;26,15,8,25;,
  4;27,14,15,26;,
  4;28,13,14,27;,
  4;29,12,13,28;,
  4;30,11,12,29;,
  4;31,10,11,30;,
  4;32,9,10,31;,
  4;25,8,9,32;,
  4;0,33,34,1;,
  4;1,34,35,2;,
  4;2,35,36,3;,
  4;3,36,37,4;,
  4;4,37,38,5;,
  4;5,38,39,6;,
  4;6,39,40,7;,
  4;7,40,33,0;,
  4;41,41,42,42;,
  4;42,42,43,43;,
  4;43,43,44,44;,
  4;44,44,45,45;,
  4;45,45,46,46;,
  4;46,46,47,47;,
  4;47,47,48,48;,
  4;48,48,41,41;,
  4;65,65,66,66;,
  4;67,67,65,65;,
  4;68,68,67,67;,
  4;69,69,68,68;,
  4;70,70,69,69;,
  4;71,71,70,70;,
  4;72,72,71,71;,
  4;66,66,72,72;,
  4;8,57,58,9;,
  4;9,58,59,10;,
  4;10,59,60,11;,
  4;11,60,61,12;,
  4;12,61,62,13;,
  4;13,62,63,14;,
  4;14,63,64,15;,
  4;15,64,57,8;;
 }
 MeshTextureCoords {
  74;
  0.881779;0.566944;,
  0.836659;0.572960;,
  0.836601;0.608796;,
  0.881711;0.608869;,
  0.836543;0.644632;,
  0.881643;0.650794;,
  0.836519;0.659475;,
  0.881615;0.668160;,
  0.836543;0.644632;,
  0.881643;0.650794;,
  0.836601;0.608796;,
  0.881711;0.608869;,
  0.836659;0.572960;,
  0.881779;0.566944;,
  0.836682;0.558117;,
  0.881807;0.549578;,
  0.747672;0.152007;,
  0.719237;0.152007;,
  0.739343;0.168575;,
  0.739343;0.135439;,
  0.719237;0.128576;,
  0.699132;0.135439;,
  0.690803;0.152007;,
  0.699132;0.168575;,
  0.719237;0.175416;,
  0.771741;0.592481;,
  0.765612;0.608681;,
  0.771715;0.608691;,
  0.771689;0.624902;,
  0.771678;0.631616;,
  0.771689;0.624902;,
  0.771715;0.608691;,
  0.771741;0.592481;,
  0.771752;0.585766;,
  0.928273;0.561445;,
  0.928197;0.608944;,
  0.928120;0.656443;,
  0.928088;0.676118;,
  0.928120;0.656443;,
  0.928197;0.608944;,
  0.928273;0.561445;,
  0.928305;0.541770;,
  0.788365;0.570628;,
  0.788346;0.581784;,
  0.788347;0.581784;,
  0.788303;0.608718;,
  0.788260;0.635652;,
  0.788242;0.646808;,
  0.788260;0.635652;,
  0.788303;0.608718;,
  0.984729;0.557924;,
  0.984647;0.609035;,
  0.984564;0.660146;,
  0.984530;0.681317;,
  0.984564;0.660146;,
  0.984647;0.609035;,
  0.984729;0.557924;,
  0.984764;0.536753;,
  0.984725;0.560903;,
  0.984757;0.540967;,
  0.984725;0.560903;,
  0.984647;0.609035;,
  0.984569;0.657167;,
  0.984537;0.677104;,
  0.984569;0.657167;,
  0.984647;0.609035;,
  0.748346;0.159759;,
  0.739821;0.176720;,
  0.739821;0.142797;,
  0.719237;0.135771;,
  0.698655;0.142797;,
  0.690129;0.159759;,
  0.698655;0.176720;,
  0.719237;0.183746;;
 }
}
