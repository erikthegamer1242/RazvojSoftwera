/** @type {import('tailwindcss').Config} */

module.exports = {
	content: [
		"./www/*.{html,js}",
		"./www/js/*.{html,js}",
	],
	theme: {
		extend: {
			colors: {
				transparent: "transparent",
				current: "currentColor",
				black: "#000",
				white: "#fff",
				gray: {
					50: "#f9fafb",
					100: "#f4f5f7",
					200: "#e5e7eb",
					300: "#d2d6dc",
					400: "#9fa6b2",
					500: "#6b7280",
					600: "#4b5563",
					700: "#374151",
					800: "#252f3f",
					900: "#161e2e",
				},
				red: {
					50: "#fef2f2",
					100: "#fee2e2",
					200: "#fecaca",
					
					300: "#fca5a5",
					400: "#f87171",
					500: "#ef4444",
					600: "#dc2626",
					700: "#b91c1c",
					800: "#991b1b",
					900: "#7f1d1d",
				},
				orange: {
					50: "#fff7ed",
					100: "#ffedd5",
					200: "#fed7aa",
					300: "#fdba74",
					400: "#fb923c",
					500: "#f97316",
					
					600: "#ea580c",
					700: "#c2410c",
					800: "#9a3412",
					900: "#7c2d12",
					
				},
				yellow: {
					50: "#fffbeb",
					100: "#fef3c7",
					200: "#fde68a",
					300: "#fcd34d",
					400: "#fbbf24",
					500: "#f59e0b",
					600: "#d97706",
					700: "#b45309",
					800: "#92400e",
					900: "#78350f",
				},
				green: {
					50: "#ecfdf5",
					100: "#d1fae5",
					200: "#a7f3d0",
					300: "#6ee7b7",
					400: "#34d399",
					500: "#10b981",
					600: "#059669",
					700: "#047857",
					800: "#065f46",
					900: "#064e3b",
				},
				blue: {
					50: "#eff6ff",
					100: "#dbeafe",
					200: "#bfdbfe",
					300: "#93c5fd",
					400: "#60a5fa",
					500: "#3b82f6",
					600: "#2563eb",
					700: "#1d4ed8",
					800: "#1e40af",
					900: "#1e3a8a",
				},
				indigo: {
					50: "#eef2ff",
					100: "#e0e7ff",
					200: "#c7d2fe",
					300: "#a5b4fc",
					400: "#818cf8",
					500: "#6366f1",
					600: "#4f46e5",
					700: "#4338ca",
					800: "#3730a3",
					900: "#312e81",
				},
				purple: {
					50: "#f5f3ff",
					100: "#ede9fe",
					200: "#ddd6fe",
					300: "#c4b5fd",
					400: "#a78bfa",
					
					500: "#8b5cf6",
					600: "#7c3aed",
					700: "#6d28d9",
					800: "#5b21b6",
					900: "#4c1d95",
				},
				pink: {
					50: "#fdf2f8",
					100: "#fce7f3",
					200: "#fbcfe8",
					300: "#f9a8d4",
					400: "#f472b6",
					500: "#ec4899",
					600: "#db2777",
					700: "#be185d",
					800: "#9d174d",
					900: "#831843",
				},
			},
			
			spacing: {
				px: "1px",
				0: "0px",
				
				0.5: "0.125rem",
				1: "0.25rem",
				'14': '3.5rem',
				'22': '5.5rem',
				'72': '18rem',
				'200': '50rem',
				
			},
			display: {
				'flex': 'flex',
				'flex':{
				
				},
				'inline-flex': 'inline-flex',
				'block': 'block',
				'inline-block': 'inline-block',
				'table': 'table',
				'table-row': 'table-row',
				'table-cell': 'table-cell',
				'hidden': 'none',
			},
			
			
			width: {
				'7/10': '70%',
				'3/10': '30%',
				'5/10': '50%',
			},
			maxWidth: {
				'7/10': '70%',
				'3/10': '30%',
				'4/10': '40%',
				'5/10': '50%',
				'6/10': '60%',
				'7/10': '70%',
				'8/10': '80%',
				'9/10': '90%',
				'10/10': '100%',
				'218': '218px',
				'10/12': '83.333333%',
				'11/12': '91.666667%',
			},
			minHeight: {
				'7/10': '70%',
				'1/10': '10%',
				'8/10': '80%',
				'218': '218px',
				'40/48': '83.333333%',
				'43/48': '91.666667%',
				'45/48': '93.75%',
				'2/20' : '10%',
				'3/20' : '15%',
				'4/20' : '20%',
				'1/20' : '5%',
				'17/20' : '85%',
			},
			height: {
				'7/10': '70%',
				'1/10': '10%',
				'5/10': '50%',
				'8/10': '80%',
				'9/10': '90%',
				'218': '218px',
				'320' : '320px',
				'60' : '60px',
				'40/48': '83.333333%',
				'43/48': '91.666667%',
				'45/48': '93.75%',
				'2/20' : '10%',
				'3/20' : '15%',
				'4/20' : '20%',
				'1/20' : '5%',
				'17/20' : '85%',
			},
			fontSize: {
				xxs: '0.5rem',
			},
			lineHeight: {
				'extra-loose': '2.5',
			},
		},
		container: {
			padding: '1rem'
		},
		fontFamily: {
			sans: [
				'-apple-system',
				'BlinkMacSystemFont',
				'"Segoe UI"',
				'Roboto',
				'"Helvetica Neue"',
				'Arial',
				'"Noto Sans"',
				'sans-serif',
				'"Apple Color Emoji"',
				'"Segoe UI Emoji"',
				'"Segoe UI Symbol"',
				'"Noto Color Emoji"',
			],
			'source-sans-pro': [
				'Source Sans Pro',
				'Roboto',
				'-apple-system',
				'BlinkMacSystemFont',
				'"Segoe UI"',
				'"Helvetica Neue"',
				'Arial',
				'"Noto Sans"',
				'sans-serif',
				'"Apple Color Emoji"',
				'"Segoe UI Emoji"',
				'"Segoe UI Symbol"',
				'"Noto Color Emoji"',
			],
			serif: ['Georgia', 'Cambria', '"Times New Roman"', 'Times', 'serif'],
			mono: [
				'Menlo',
				'Monaco',
				'Consolas',
				'"Liberation Mono"',
				'"Courier New"',
				'monospace',
			],
		},
	},
	variants: {
		textColor: ['responsive', 'hover', 'focus', 'group-hover'],
	},
}