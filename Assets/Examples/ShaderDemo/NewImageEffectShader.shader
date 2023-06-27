Shader "Custom/CircleShader" {
    Properties {
        _Color ("Color", Color) = (1, 1, 1, 1)
        _Radius ("Radius", Range(0.01, 0.5)) = 0.25
    }
    SubShader {
        Tags {"Queue"="Transparent" "RenderType"="Transparent"}
        LOD 100

        Pass {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"
            struct appdata {
                float4 vertex : POSITION;
                              float2 uv : TEXCOORD0;
            };

            struct v2f {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            float4 _Color;
            float _Radius;

            v2f vert (appdata v) {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv ;
                return o;
            }

            fixed4 frag (v2f i) : SV_Target {
                
               float thinkness=0.5;
               float fade=0.00001;
               
               float2 uv=i.uv*2-1.0;    // -1  -   1
               
               float distance=1.0 -length(uv);
               float colorA=smoothstep(0,fade,distance);
               colorA*=smoothstep(thinkness+fade,thinkness,distance);
               
               if(colorA<=0.001)
               discard;
               
               return colorA*_Color;
                  
                
            }
            ENDCG
        }
    }
}