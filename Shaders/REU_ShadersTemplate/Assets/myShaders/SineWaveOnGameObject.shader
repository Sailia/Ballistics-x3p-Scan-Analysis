Shader "REU/SineWaveOnGameObject"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
		_Color("Object color", Color) = (1,1,1,1)
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            // make fog work
            #pragma multi_compile_fog

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
				float3 normal : NORMAL;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                UNITY_FOG_COORDS(1)
				fixed4 color : COLOR;
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;

			float4 _Color;

            v2f vert (appdata v)
            {
                v2f o;

				float3 normalDirection = normalize( mul(float4(v.normal, 0.0), unity_WorldToObject).xyz);
				float3 lightDirection = normalize(_WorldSpaceLightPos0.xyz);
				float3 diffuseReflection = _Color * max(0.0, dot(normalDirection, lightDirection) );
				o.color = float4(diffuseReflection, 1.0);

				//Write code to do a sine wave offset, and add it to v.vertex below
				float4 offset = (
					0,
					0,
					0,
					sin(v.vertex.z / 50 + _Time[3]) / 2
					);

                o.vertex = UnityObjectToClipPos(v.vertex + offset);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                UNITY_TRANSFER_FOG(o,o.vertex);
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // sample the texture
                fixed4 col = tex2D(_MainTex, i.uv);
				return col * i.color;

            }
            ENDCG
        }
    }
}
